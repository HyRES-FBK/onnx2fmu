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
#endif

#define MAX_PATH_LENGTH 4096

void initializeOrtApi(ModelInstance* comp) {
    const OrtApi* g_ort = NULL;
    g_ort = OrtGetApiBase()->GetApi(ORT_API_VERSION);
    if (!g_ort) {
        logError(comp, "Failed to init ONNX Runtime engine.");
        return;
    }
    comp->g_ort = g_ort;
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

