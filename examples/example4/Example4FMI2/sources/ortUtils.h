#include "model.h"
#include "onnxruntime_c_api.h"

// Initialize the ONNX Runtime API
void initializeOrtApi(ModelInstance* comp);
// Create the environment
void createOrtEnv(ModelInstance* comp);
// Create the session options
void createOrtSessionOptions(ModelInstance* comp);
// // Create the session
void createOrtSession(OrtEnv* env, const char* model_path, OrtSessionOptions* session_options, ModelInstance* comp);
// Release the session
void freeSession(OrtSession* session, ModelInstance* comp);
// Release the session options
void freeOrtSessionOptions(OrtSessionOptions* session_options, ModelInstance* comp);
// Release the environment
void freeOrtEnv(OrtEnv* env, ModelInstance* comp);

