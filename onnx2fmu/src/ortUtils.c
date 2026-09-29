#if defined(__linux__) && !defined(_GNU_SOURCE)
// dladdr() is only declared by <dlfcn.h> when _GNU_SOURCE is defined on glibc.
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>

#include "model.h"
#include "config.h"
#include "ortUtils.h"
#include "onnxruntime_c_api.h"

#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <dlfcn.h>
#endif

#define MAX_PATH_LENGTH 4096

// Each generated FMU ships its own private copy of the ONNX Runtime library,
// renamed with the model identifier, next to the model's own binary. It is
// loaded explicitly by path (rather than linked at build time) so that the
// generic "onnxruntime" library name never appears in this binary's import
// table / DT_NEEDED / LC_LOAD_DYLIB entries: two FMUs (or the FMU and the FMI
// importer) that each embed a different ONNX Runtime version can then be
// loaded into the same process without the OS module loader conflating their
// same-named libraries. See https://github.com/HyRES-FBK/onnx2fmu/issues/53.
#ifdef _WIN32
#define ORT_LIBRARY_FILENAME XSTR(MODEL_IDENTIFIER) "_onnxruntime.dll"
#elif defined(__APPLE__)
#define ORT_LIBRARY_FILENAME "lib" XSTR(MODEL_IDENTIFIER) "_onnxruntime.dylib"
#else
#define ORT_LIBRARY_FILENAME "lib" XSTR(MODEL_IDENTIFIER) "_onnxruntime.so"
#endif

#define STR(x) #x
#define XSTR(x) STR(x)

typedef const OrtApiBase* (*OrtGetApiBaseFn)(void);

// Resolve the directory this shared library (the model's own binary) was
// loaded from, so the private ONNX Runtime library can be found as a sibling
// file regardless of the process' current working directory.
static int getOwnLibraryDirectory(char* buffer, size_t bufferSize) {
#ifdef _WIN32
    HMODULE hModule = NULL;
    char path[MAX_PATH_LENGTH];
    DWORD len;
    char* lastSep;
    size_t dirLen;

    if (!GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR)&getOwnLibraryDirectory,
            &hModule)) {
        return 0;
    }

    len = GetModuleFileNameA(hModule, path, MAX_PATH_LENGTH);
    if (len == 0 || len == MAX_PATH_LENGTH) return 0;

    lastSep = strrchr(path, '\\');
    if (!lastSep) return 0;

    dirLen = (size_t)(lastSep - path);
    if (dirLen >= bufferSize) return 0;

    memcpy(buffer, path, dirLen);
    buffer[dirLen] = '\0';
    return 1;
#else
    Dl_info info;
    char path[MAX_PATH_LENGTH];
    char* lastSep;
    size_t dirLen;
    union { void (*fn)(void); void* obj; } self;

    self.fn = (void (*)(void))&getOwnLibraryDirectory;

    if (!dladdr(self.obj, &info) || !info.dli_fname) {
        return 0;
    }

    strncpy(path, info.dli_fname, MAX_PATH_LENGTH - 1);
    path[MAX_PATH_LENGTH - 1] = '\0';

    lastSep = strrchr(path, '/');
    if (!lastSep) return 0;

    dirLen = (size_t)(lastSep - path);
    if (dirLen >= bufferSize) return 0;

    memcpy(buffer, path, dirLen);
    buffer[dirLen] = '\0';
    return 1;
#endif
}

// Load this FMU's private ONNX Runtime library and resolve its sole entry
// point. Returns the library handle (to be released with freeOrtLibrary) and
// writes the resolved function into *outFn, or returns NULL on failure.
static void* loadOrtLibrary(ModelInstance* comp, OrtGetApiBaseFn* outFn) {
    char dir[MAX_PATH_LENGTH];
    char path[MAX_PATH_LENGTH];
    int written;

    if (!getOwnLibraryDirectory(dir, sizeof(dir))) {
        logError(comp, "Failed to determine the location of the model's own binary.");
        return NULL;
    }

#ifdef _WIN32
    written = snprintf(path, sizeof(path), "%s\\%s", dir, ORT_LIBRARY_FILENAME);
#else
    written = snprintf(path, sizeof(path), "%s/%s", dir, ORT_LIBRARY_FILENAME);
#endif
    if (written < 0 || (size_t)written >= sizeof(path)) {
        logError(comp, "ONNX Runtime library path is too long.");
        return NULL;
    }

#ifdef _WIN32
    {
        HMODULE handle = LoadLibraryExA(path, NULL, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        FARPROC sym;

        if (!handle) {
            logError(comp, "Failed to load private ONNX Runtime library: %s", path);
            return NULL;
        }

        sym = GetProcAddress(handle, "OrtGetApiBase");
        if (!sym) {
            logError(comp, "Failed to resolve OrtGetApiBase in %s", path);
            FreeLibrary(handle);
            return NULL;
        }

        *outFn = (OrtGetApiBaseFn)sym;
        return (void*)handle;
    }
#else
    {
        void* handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
        union { void* obj; OrtGetApiBaseFn fn; } sym;

        if (!handle) {
            logError(comp, "Failed to load private ONNX Runtime library: %s (%s)", path, dlerror());
            return NULL;
        }

        sym.obj = dlsym(handle, "OrtGetApiBase");
        if (!sym.obj) {
            logError(comp, "Failed to resolve OrtGetApiBase in %s (%s)", path, dlerror());
            dlclose(handle);
            return NULL;
        }

        *outFn = sym.fn;
        return handle;
    }
#endif
}

void initializeOrtApi(ModelInstance* comp) {
    OrtGetApiBaseFn ortGetApiBase = NULL;
    void* handle = loadOrtLibrary(comp, &ortGetApiBase);
    const OrtApi* g_ort = NULL;

    if (!handle) {
        return; // error already logged
    }
    comp->ortLibraryHandle = handle;

    g_ort = ortGetApiBase()->GetApi(ORT_API_VERSION);
    if (!g_ort) {
        const char *version = ortGetApiBase()->GetVersionString();
        logError(comp, "Failed to init ONNX Runtime engine: get '%s' instead of '%d'", version, ORT_API_VERSION);
        return;
    }
    comp->g_ort = g_ort;
}

void freeOrtLibrary(ModelInstance* comp) {
    if (!comp->ortLibraryHandle) return;

#ifdef _WIN32
    FreeLibrary((HMODULE)comp->ortLibraryHandle);
#else
    dlclose(comp->ortLibraryHandle);
#endif

    comp->ortLibraryHandle = NULL;
    logEvent(comp, "ONNX Runtime library unloaded.");
}

void createOrtEnv(ModelInstance* comp) {
    OrtEnv* env = NULL;
    OrtStatus* status = comp->g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "test", &env);
    if (status != NULL) {
        const char* msg = comp->g_ort->GetErrorMessage(status);
        logError(comp, msg);
        comp->g_ort->ReleaseStatus(status);
        return;
    }
    comp->env = env;
    logEvent(comp, "ONNX Runtime environment created.");
}

void createOrtSessionOptions(ModelInstance* comp) {
    OrtSessionOptions* session_options = NULL;
    OrtStatus* status = comp->g_ort->CreateSessionOptions(&session_options);
    if (status != NULL) {
        const char* msg = comp->g_ort->GetErrorMessage(status);
        logError(comp, msg);
        comp->g_ort->ReleaseStatus(status);
        return;
    }
    comp->session_options = session_options;
    logEvent(comp, "ONNX Runtime session options created.");
}

void createOrtSession(OrtEnv* env, const char* resourceLocation, OrtSessionOptions* session_options, ModelInstance* comp) {
    // Resource location check, see https://github.com/modelica/Reference-FMUs/blob/main/Resource/model.c
    char path[MAX_PATH_LENGTH] = "";

    if (!resourceLocation) {
        logError(comp, "Resource location must not be NULL.");
        return;
    }

    logEvent(comp, "Resource location: %s", resourceLocation);

#ifdef _WIN32

#if FMI_VERSION < 3
    DWORD pathLen = MAX_PATH_LENGTH;

    if (PathCreateFromUrlA(resourceLocation, path, &pathLen, 0) != S_OK) {
        logError(comp, "Failed to convert resource location to file system path.");
    }
#else
    strncpy(path, resourceLocation, MAX_PATH_LENGTH);
#endif

#if FMI_VERSION == 1
    if (!PathAppendA(path, "resources") || !PathAppendA(path, "model.onnx")) return;
#elif FMI_VERSION == 2
    if (!PathAppendA(path, "model.onnx")) return;
#else
    if (!strncat(path, "model.onnx", MAX_PATH_LENGTH)) return;
#endif

#else

#if FMI_VERSION < 3
    const char *scheme1 = "file:///";
    const char *scheme2 = "file:/";

    if (strncmp(resourceLocation, scheme1, strlen(scheme1)) == 0) {
        strncpy(path, &resourceLocation[strlen(scheme1)] - 1, MAX_PATH_LENGTH-1);
    } else if (strncmp(resourceLocation, scheme2, strlen(scheme2)) == 0) {
        strncpy(path, &resourceLocation[strlen(scheme2) - 1], MAX_PATH_LENGTH-1);
    } else {
        logError(comp, "The resourceLocation must start with \"file:/\" or \"file:///\"");
    }

    // decode percent encoded characters
    char* src = path;
    char* dst = path;

    char buf[3] = { '\0', '\0', '\0' };

    while (*src) {

        if (*src == '%' && (buf[0] = src[1]) && (buf[1] = src[2])) {
            *dst = strtol(buf, NULL, 16);
            src += 3;
        } else {
            *dst = *src;
            src++;
        }

        dst++;
    }

    *dst = '\0';
#else
    strncpy(path, resourceLocation, MAX_PATH_LENGTH);
#endif

    logEvent(comp, "Path: %s", path);

#if FMI_VERSION == 1
    strncat(path, "/resources/model.onnx", MAX_PATH_LENGTH-strlen(path)-1);
#elif FMI_VERSION == 2
    strncat(path, "/model.onnx", MAX_PATH_LENGTH-strlen(path)-1);
#else
    strncat(path, "/model.onnx", MAX_PATH_LENGTH-strlen(path)-1);
#endif
    path[MAX_PATH_LENGTH-1] = 0;

#endif

    logEvent(comp, "Model path: %s", path);

#if _WIN32
    // Convert char path to wchar_t path
    wchar_t wpath[MAX_PATH_LENGTH];
    mbstowcs(wpath, path, MAX_PATH_LENGTH);

    OrtSession* session = NULL;
    OrtStatus* status = comp->g_ort->CreateSession(env, wpath, session_options, &session);

    if (status != NULL) {
        const char* msg = comp->g_ort->GetErrorMessage(status);
        logError(comp, msg);
        comp->g_ort->ReleaseStatus(status);
        return;
    }
#else
    OrtSession* session = NULL;
    OrtStatus* status = comp->g_ort->CreateSession(env, path, session_options, &session);

    if (status != NULL) {
        const char* msg = comp->g_ort->GetErrorMessage(status);
        logError(comp, msg);
        comp->g_ort->ReleaseStatus(status);
        return;
    }
#endif

    comp->session = session;

    logEvent(comp, "ONNX Runtime session created.");

    return;
}

void freeSession(OrtSession* session, ModelInstance* comp) {
    comp->g_ort->ReleaseSession(session);
    logEvent(comp, "ONNX Runtime session released.");
}

void freeOrtSessionOptions(OrtSessionOptions* session_options, ModelInstance* comp) {
    comp->g_ort->ReleaseSessionOptions(session_options);
    logEvent(comp, "ONNX Runtime session options released.");
}

void freeOrtEnv(OrtEnv* env, ModelInstance* comp) {
    comp->g_ort->ReleaseEnv(env);
    logEvent(comp, "ONNX Runtime environment released.");
}

