#include"config.h"
#include "model.h"
#include "onnxruntime_c_api.h"

#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

#define MAX_PATH_LENGTH 4096

#define ORT_ABORT_ON_ERROR(expr, comp) \
    do { \
    OrtStatus* onnx_status = (expr); \
    if (onnx_status != NULL) { \
            const char* msg = comp->g_ort->GetErrorMessage(onnx_status); \
            logError(comp, "%s\n", msg); \
            comp->g_ort->ReleaseStatus(onnx_status); \
        } \
    } while (0);

void setStartValues(ModelInstance *comp) {
    // Input variables
    M(u_0_0) = (double) 0.0;
    M(u_0_1) = (double) 0.0;
    M(u_0_2) = (double) 0.0;
    M(u_0_3) = (double) 0.0;
    M(u_0_4) = (double) 0.0;
    // Local variables
    M(X_X1_0_0) = (double) 1.0;
    M(X_X1_0_1) = (double) 1.0;
    M(X_X1_0_2) = (double) 1.0;
    M(X_X1_1_0) = (double) 1.0;
    M(X_X1_1_1) = (double) 1.0;
    M(X_X1_1_2) = (double) 1.0;
    M(X_X1_2_0) = (double) 1.0;
    M(X_X1_2_1) = (double) 1.0;
    M(X_X1_2_2) = (double) 1.0;
    M(X_X1_3_0) = (double) 1.0;
    M(X_X1_3_1) = (double) 1.0;
    M(X_X1_3_2) = (double) 1.0;
    M(X_X1_4_0) = (double) 1.0;
    M(X_X1_4_1) = (double) 1.0;
    M(X_X1_4_2) = (double) 1.0;
    M(X_X1_5_0) = (double) 1.0;
    M(X_X1_5_1) = (double) 1.0;
    M(X_X1_5_2) = (double) 1.0;
    M(X_X1_6_0) = (double) 1.0;
    M(X_X1_6_1) = (double) 1.0;
    M(X_X1_6_2) = (double) 1.0;
    M(X_X1_7_0) = (double) 1.0;
    M(X_X1_7_1) = (double) 1.0;
    M(X_X1_7_2) = (double) 1.0;
    M(X_X1_8_0) = (double) 1.0;
    M(X_X1_8_1) = (double) 1.0;
    M(X_X1_8_2) = (double) 1.0;
    M(X_X1_9_0) = (double) 1.0;
    M(X_X1_9_1) = (double) 1.0;
    M(X_X1_9_2) = (double) 1.0;
    M(U_U1_0_0) = (double) 0.0;
    M(U_U1_0_1) = (double) 0.0;
    M(U_U1_0_2) = (double) 0.0;
    M(U_U1_0_3) = (double) 0.0;
    M(U_U1_0_4) = (double) 0.0;
    M(U_U1_1_0) = (double) 0.0;
    M(U_U1_1_1) = (double) 0.0;
    M(U_U1_1_2) = (double) 0.0;
    M(U_U1_1_3) = (double) 0.0;
    M(U_U1_1_4) = (double) 0.0;
    M(U_U1_2_0) = (double) 0.0;
    M(U_U1_2_1) = (double) 0.0;
    M(U_U1_2_2) = (double) 0.0;
    M(U_U1_2_3) = (double) 0.0;
    M(U_U1_2_4) = (double) 0.0;
    M(U_U1_3_0) = (double) 0.0;
    M(U_U1_3_1) = (double) 0.0;
    M(U_U1_3_2) = (double) 0.0;
    M(U_U1_3_3) = (double) 0.0;
    M(U_U1_3_4) = (double) 0.0;
    M(U_U1_4_0) = (double) 0.0;
    M(U_U1_4_1) = (double) 0.0;
    M(U_U1_4_2) = (double) 0.0;
    M(U_U1_4_3) = (double) 0.0;
    M(U_U1_4_4) = (double) 0.0;
    M(U_U1_5_0) = (double) 0.0;
    M(U_U1_5_1) = (double) 0.0;
    M(U_U1_5_2) = (double) 0.0;
    M(U_U1_5_3) = (double) 0.0;
    M(U_U1_5_4) = (double) 0.0;
    M(U_U1_6_0) = (double) 0.0;
    M(U_U1_6_1) = (double) 0.0;
    M(U_U1_6_2) = (double) 0.0;
    M(U_U1_6_3) = (double) 0.0;
    M(U_U1_6_4) = (double) 0.0;
    M(U_U1_7_0) = (double) 0.0;
    M(U_U1_7_1) = (double) 0.0;
    M(U_U1_7_2) = (double) 0.0;
    M(U_U1_7_3) = (double) 0.0;
    M(U_U1_7_4) = (double) 0.0;
    M(U_U1_8_0) = (double) 0.0;
    M(U_U1_8_1) = (double) 0.0;
    M(U_U1_8_2) = (double) 0.0;
    M(U_U1_8_3) = (double) 0.0;
    M(U_U1_8_4) = (double) 0.0;
}

Status calculateValues(ModelInstance *comp) {

    // Do I need memory info?
    OrtMemoryInfo* memory_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateCpuMemoryInfo(
            OrtArenaAllocator, OrtMemTypeDefault, &memory_info
        ),
        comp);

    // Create u tensor
    OrtValue* u_tensor;

    // Store the shape of the input tensor
    const size_t u_shape[] = { 1, 5 };

    // Determine the dimensions of the input tensor
    const size_t u_dim = sizeof(u_shape) / sizeof(u_shape[0]);
    size_t u_size = 1;
    for (size_t i = 0; i < u_dim; ++i) {
        u_size *= u_shape[i];
    }

    // Store values in the flattened array
    float* u_float = (float*)malloc(u_size * sizeof(float));
    if (u_float == NULL) {
        logError(comp, "Failed to allocate memory for u_float");
        return Error;
    }

    // Flatten the input array
    u_float[0] = (float)M(u_0_0);
    u_float[1] = (float)M(u_0_1);
    u_float[2] = (float)M(u_0_2);
    u_float[3] = (float)M(u_0_3);
    u_float[4] = (float)M(u_0_4);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            u_float,
            u_size * sizeof(float),
            (const int64_t*)u_shape,
            u_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &u_tensor
        ),
        comp);

    int u_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            u_tensor,
            &u_is_tensor),
            comp
        );

    assert(u_is_tensor);

    // LOCAL variables

    // Create X tensor
    OrtValue* X_tensor;

    // Store the shape of the input tensor
    const size_t X_shape[] = { 10, 3 };

    // Determine the dimensions of the local tensor
    const size_t X_dim = sizeof(X_shape) / sizeof(X_shape[0]);
    size_t X_size = 1;
    for (size_t i = 0; i < X_dim; ++i) {
        X_size *= X_shape[i];
    }

    // Store values in the flattened array
    float* X_float = (float*)malloc(X_size * sizeof(float));
    if (X_float == NULL) {
        logError(comp, "Failed to allocate memory for X_float");
        return Error;
    }

    // Flatten the local array
    X_float[0] = (float)M(X_X1_0_0);
    X_float[1] = (float)M(X_X1_0_1);
    X_float[2] = (float)M(X_X1_0_2);
    X_float[3] = (float)M(X_X1_1_0);
    X_float[4] = (float)M(X_X1_1_1);
    X_float[5] = (float)M(X_X1_1_2);
    X_float[6] = (float)M(X_X1_2_0);
    X_float[7] = (float)M(X_X1_2_1);
    X_float[8] = (float)M(X_X1_2_2);
    X_float[9] = (float)M(X_X1_3_0);
    X_float[10] = (float)M(X_X1_3_1);
    X_float[11] = (float)M(X_X1_3_2);
    X_float[12] = (float)M(X_X1_4_0);
    X_float[13] = (float)M(X_X1_4_1);
    X_float[14] = (float)M(X_X1_4_2);
    X_float[15] = (float)M(X_X1_5_0);
    X_float[16] = (float)M(X_X1_5_1);
    X_float[17] = (float)M(X_X1_5_2);
    X_float[18] = (float)M(X_X1_6_0);
    X_float[19] = (float)M(X_X1_6_1);
    X_float[20] = (float)M(X_X1_6_2);
    X_float[21] = (float)M(X_X1_7_0);
    X_float[22] = (float)M(X_X1_7_1);
    X_float[23] = (float)M(X_X1_7_2);
    X_float[24] = (float)M(X_X1_8_0);
    X_float[25] = (float)M(X_X1_8_1);
    X_float[26] = (float)M(X_X1_8_2);
    X_float[27] = (float)M(X_X1_9_0);
    X_float[28] = (float)M(X_X1_9_1);
    X_float[29] = (float)M(X_X1_9_2);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            X_float,
            X_size * sizeof(float),
            (const int64_t*)X_shape,
            X_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &X_tensor
        ),
        comp);

    int X_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            X_tensor,
            &X_is_tensor),
            comp
        );

    assert(X_is_tensor);

    // Create U tensor
    OrtValue* U_tensor;

    // Store the shape of the input tensor
    const size_t U_shape[] = { 9, 5 };

    // Determine the dimensions of the local tensor
    const size_t U_dim = sizeof(U_shape) / sizeof(U_shape[0]);
    size_t U_size = 1;
    for (size_t i = 0; i < U_dim; ++i) {
        U_size *= U_shape[i];
    }

    // Store values in the flattened array
    float* U_float = (float*)malloc(U_size * sizeof(float));
    if (U_float == NULL) {
        logError(comp, "Failed to allocate memory for U_float");
        return Error;
    }

    // Flatten the local array
    U_float[0] = (float)M(U_U1_0_0);
    U_float[1] = (float)M(U_U1_0_1);
    U_float[2] = (float)M(U_U1_0_2);
    U_float[3] = (float)M(U_U1_0_3);
    U_float[4] = (float)M(U_U1_0_4);
    U_float[5] = (float)M(U_U1_1_0);
    U_float[6] = (float)M(U_U1_1_1);
    U_float[7] = (float)M(U_U1_1_2);
    U_float[8] = (float)M(U_U1_1_3);
    U_float[9] = (float)M(U_U1_1_4);
    U_float[10] = (float)M(U_U1_2_0);
    U_float[11] = (float)M(U_U1_2_1);
    U_float[12] = (float)M(U_U1_2_2);
    U_float[13] = (float)M(U_U1_2_3);
    U_float[14] = (float)M(U_U1_2_4);
    U_float[15] = (float)M(U_U1_3_0);
    U_float[16] = (float)M(U_U1_3_1);
    U_float[17] = (float)M(U_U1_3_2);
    U_float[18] = (float)M(U_U1_3_3);
    U_float[19] = (float)M(U_U1_3_4);
    U_float[20] = (float)M(U_U1_4_0);
    U_float[21] = (float)M(U_U1_4_1);
    U_float[22] = (float)M(U_U1_4_2);
    U_float[23] = (float)M(U_U1_4_3);
    U_float[24] = (float)M(U_U1_4_4);
    U_float[25] = (float)M(U_U1_5_0);
    U_float[26] = (float)M(U_U1_5_1);
    U_float[27] = (float)M(U_U1_5_2);
    U_float[28] = (float)M(U_U1_5_3);
    U_float[29] = (float)M(U_U1_5_4);
    U_float[30] = (float)M(U_U1_6_0);
    U_float[31] = (float)M(U_U1_6_1);
    U_float[32] = (float)M(U_U1_6_2);
    U_float[33] = (float)M(U_U1_6_3);
    U_float[34] = (float)M(U_U1_6_4);
    U_float[35] = (float)M(U_U1_7_0);
    U_float[36] = (float)M(U_U1_7_1);
    U_float[37] = (float)M(U_U1_7_2);
    U_float[38] = (float)M(U_U1_7_3);
    U_float[39] = (float)M(U_U1_7_4);
    U_float[40] = (float)M(U_U1_8_0);
    U_float[41] = (float)M(U_U1_8_1);
    U_float[42] = (float)M(U_U1_8_2);
    U_float[43] = (float)M(U_U1_8_3);
    U_float[44] = (float)M(U_U1_8_4);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            U_float,
            U_size * sizeof(float),
            (const int64_t*)U_shape,
            U_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &U_tensor
        ),
        comp);

    int U_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            U_tensor,
            &U_is_tensor),
            comp
        );

    assert(U_is_tensor);

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Create output tensors
    OrtValue* x_tensor = NULL;
    // Create output tensors for local variables
    OrtValue* X1_tensor = NULL;
    OrtValue* U1_tensor = NULL;


    // Declare input node names
    const char* input_names[] = {
        "u",
        "X",
        "U"
    };

    // Declare output node names
    const char* output_names[] = {
        "x",
        "X1",
        "U1"
    };

    // Gather input tensors
    const OrtValue* input_tensors[] = {
        u_tensor,
        X_tensor,
        U_tensor
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        x_tensor,
        X1_tensor,
        U1_tensor
    };

    // Run inference
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(
            comp->session,
            NULL,
            input_names,
            input_tensors,
            3,
            output_names,
            3,
            output_tensors
        ),
        comp
    );

    // Check output tensors to be tensors
    int x_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[0],
            &x_is_tensor),
            comp
        );
    assert (x_is_tensor);

    // Check local output tensors to be tensors
    int X1_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[1],
            &X1_is_tensor),
            comp
        );
    assert (X1_is_tensor);
    int U1_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[2],
            &U1_is_tensor),
            comp
        );
    assert (U1_is_tensor);

    // Retrieve pointer to the x tensor
    float* x_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[0],
            (void**)&x_tensor_data
        ),
        comp
    );

    // Retrieve x tensor info
    OrtTensorTypeAndShapeInfo* x_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[0],
            &x_info
        ),
        comp
    );

    // Retrieve x tensor shape
    size_t x_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            x_info,
            &x_dims
        ),
        comp
    );

    // Set x tensor data to model
    M(x_0_0) = x_tensor_data[0];
    M(x_0_1) = x_tensor_data[1];
    M(x_0_2) = x_tensor_data[2];

    // Retrieve pointer to the X1 tensor
    float* X1_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[1],
            (void**)&X1_tensor_data
        ),
        comp
    );

    // Retrieve X1 tensor info
    OrtTensorTypeAndShapeInfo* X1_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[1],
            &X1_info
        ),
        comp
    );

    // Retrieve X_X1 tensor shape
    size_t X1_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            X1_info,
            &X1_dims
        ),
        comp
    );

    // Set X1 tensor data to model
    M(X_X1_0_0) = X1_tensor_data[0];
    M(X_X1_0_1) = X1_tensor_data[1];
    M(X_X1_0_2) = X1_tensor_data[2];
    M(X_X1_1_0) = X1_tensor_data[3];
    M(X_X1_1_1) = X1_tensor_data[4];
    M(X_X1_1_2) = X1_tensor_data[5];
    M(X_X1_2_0) = X1_tensor_data[6];
    M(X_X1_2_1) = X1_tensor_data[7];
    M(X_X1_2_2) = X1_tensor_data[8];
    M(X_X1_3_0) = X1_tensor_data[9];
    M(X_X1_3_1) = X1_tensor_data[10];
    M(X_X1_3_2) = X1_tensor_data[11];
    M(X_X1_4_0) = X1_tensor_data[12];
    M(X_X1_4_1) = X1_tensor_data[13];
    M(X_X1_4_2) = X1_tensor_data[14];
    M(X_X1_5_0) = X1_tensor_data[15];
    M(X_X1_5_1) = X1_tensor_data[16];
    M(X_X1_5_2) = X1_tensor_data[17];
    M(X_X1_6_0) = X1_tensor_data[18];
    M(X_X1_6_1) = X1_tensor_data[19];
    M(X_X1_6_2) = X1_tensor_data[20];
    M(X_X1_7_0) = X1_tensor_data[21];
    M(X_X1_7_1) = X1_tensor_data[22];
    M(X_X1_7_2) = X1_tensor_data[23];
    M(X_X1_8_0) = X1_tensor_data[24];
    M(X_X1_8_1) = X1_tensor_data[25];
    M(X_X1_8_2) = X1_tensor_data[26];
    M(X_X1_9_0) = X1_tensor_data[27];
    M(X_X1_9_1) = X1_tensor_data[28];
    M(X_X1_9_2) = X1_tensor_data[29];

    // Retrieve pointer to the U1 tensor
    float* U1_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[2],
            (void**)&U1_tensor_data
        ),
        comp
    );

    // Retrieve U1 tensor info
    OrtTensorTypeAndShapeInfo* U1_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[2],
            &U1_info
        ),
        comp
    );

    // Retrieve U_U1 tensor shape
    size_t U1_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            U1_info,
            &U1_dims
        ),
        comp
    );

    // Set U1 tensor data to model
    M(U_U1_0_0) = U1_tensor_data[0];
    M(U_U1_0_1) = U1_tensor_data[1];
    M(U_U1_0_2) = U1_tensor_data[2];
    M(U_U1_0_3) = U1_tensor_data[3];
    M(U_U1_0_4) = U1_tensor_data[4];
    M(U_U1_1_0) = U1_tensor_data[5];
    M(U_U1_1_1) = U1_tensor_data[6];
    M(U_U1_1_2) = U1_tensor_data[7];
    M(U_U1_1_3) = U1_tensor_data[8];
    M(U_U1_1_4) = U1_tensor_data[9];
    M(U_U1_2_0) = U1_tensor_data[10];
    M(U_U1_2_1) = U1_tensor_data[11];
    M(U_U1_2_2) = U1_tensor_data[12];
    M(U_U1_2_3) = U1_tensor_data[13];
    M(U_U1_2_4) = U1_tensor_data[14];
    M(U_U1_3_0) = U1_tensor_data[15];
    M(U_U1_3_1) = U1_tensor_data[16];
    M(U_U1_3_2) = U1_tensor_data[17];
    M(U_U1_3_3) = U1_tensor_data[18];
    M(U_U1_3_4) = U1_tensor_data[19];
    M(U_U1_4_0) = U1_tensor_data[20];
    M(U_U1_4_1) = U1_tensor_data[21];
    M(U_U1_4_2) = U1_tensor_data[22];
    M(U_U1_4_3) = U1_tensor_data[23];
    M(U_U1_4_4) = U1_tensor_data[24];
    M(U_U1_5_0) = U1_tensor_data[25];
    M(U_U1_5_1) = U1_tensor_data[26];
    M(U_U1_5_2) = U1_tensor_data[27];
    M(U_U1_5_3) = U1_tensor_data[28];
    M(U_U1_5_4) = U1_tensor_data[29];
    M(U_U1_6_0) = U1_tensor_data[30];
    M(U_U1_6_1) = U1_tensor_data[31];
    M(U_U1_6_2) = U1_tensor_data[32];
    M(U_U1_6_3) = U1_tensor_data[33];
    M(U_U1_6_4) = U1_tensor_data[34];
    M(U_U1_7_0) = U1_tensor_data[35];
    M(U_U1_7_1) = U1_tensor_data[36];
    M(U_U1_7_2) = U1_tensor_data[37];
    M(U_U1_7_3) = U1_tensor_data[38];
    M(U_U1_7_4) = U1_tensor_data[39];
    M(U_U1_8_0) = U1_tensor_data[40];
    M(U_U1_8_1) = U1_tensor_data[41];
    M(U_U1_8_2) = U1_tensor_data[42];
    M(U_U1_8_3) = U1_tensor_data[43];
    M(U_U1_8_4) = U1_tensor_data[44];

    // Free tensors
    comp->g_ort->ReleaseValue(X1_tensor);
    comp->g_ort->ReleaseValue(U1_tensor);
    comp->g_ort->ReleaseValue(x_tensor);
    comp->g_ort->ReleaseValue(X_tensor);
    comp->g_ort->ReleaseValue(U_tensor);
    comp->g_ort->ReleaseValue(u_tensor);

    // Free arrays
    free(X_float);
    free(U_float);
    free(u_float);

    return OK;
}

Status getFloat64(ModelInstance *comp, ValueReference vr, double values[],
                  size_t nValues, size_t *index) {

    switch (vr)
    {
    case vr_time:
        ASSERT_NVALUES(1);
        values[(*index)++] = M(time);
        return OK;
#if FMI_VERSION < 3
        // Inputs
        case vr_u_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_0);
            return OK;
        case vr_u_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_1);
            return OK;
        case vr_u_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_2);
            return OK;
        case vr_u_0_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_3);
            return OK;
        case vr_u_0_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_4);
            return OK;
        // Outputs
        case vr_x_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_0_0);
            return OK;
        case vr_x_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_0_1);
            return OK;
        case vr_x_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_0_2);
            return OK;
        // Locals
        case vr_X_X1_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_0_0);
            return OK;
        case vr_X_X1_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_0_1);
            return OK;
        case vr_X_X1_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_0_2);
            return OK;
        case vr_X_X1_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_1_0);
            return OK;
        case vr_X_X1_1_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_1_1);
            return OK;
        case vr_X_X1_1_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_1_2);
            return OK;
        case vr_X_X1_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_2_0);
            return OK;
        case vr_X_X1_2_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_2_1);
            return OK;
        case vr_X_X1_2_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_2_2);
            return OK;
        case vr_X_X1_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_3_0);
            return OK;
        case vr_X_X1_3_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_3_1);
            return OK;
        case vr_X_X1_3_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_3_2);
            return OK;
        case vr_X_X1_4_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_4_0);
            return OK;
        case vr_X_X1_4_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_4_1);
            return OK;
        case vr_X_X1_4_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_4_2);
            return OK;
        case vr_X_X1_5_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_5_0);
            return OK;
        case vr_X_X1_5_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_5_1);
            return OK;
        case vr_X_X1_5_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_5_2);
            return OK;
        case vr_X_X1_6_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_6_0);
            return OK;
        case vr_X_X1_6_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_6_1);
            return OK;
        case vr_X_X1_6_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_6_2);
            return OK;
        case vr_X_X1_7_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_7_0);
            return OK;
        case vr_X_X1_7_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_7_1);
            return OK;
        case vr_X_X1_7_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_7_2);
            return OK;
        case vr_X_X1_8_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_8_0);
            return OK;
        case vr_X_X1_8_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_8_1);
            return OK;
        case vr_X_X1_8_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_8_2);
            return OK;
        case vr_X_X1_9_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_9_0);
            return OK;
        case vr_X_X1_9_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_9_1);
            return OK;
        case vr_X_X1_9_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_9_2);
            return OK;
        case vr_U_U1_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_0);
            return OK;
        case vr_U_U1_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_1);
            return OK;
        case vr_U_U1_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_2);
            return OK;
        case vr_U_U1_0_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_3);
            return OK;
        case vr_U_U1_0_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_4);
            return OK;
        case vr_U_U1_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_0);
            return OK;
        case vr_U_U1_1_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_1);
            return OK;
        case vr_U_U1_1_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_2);
            return OK;
        case vr_U_U1_1_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_3);
            return OK;
        case vr_U_U1_1_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_4);
            return OK;
        case vr_U_U1_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_0);
            return OK;
        case vr_U_U1_2_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_1);
            return OK;
        case vr_U_U1_2_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_2);
            return OK;
        case vr_U_U1_2_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_3);
            return OK;
        case vr_U_U1_2_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_4);
            return OK;
        case vr_U_U1_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_0);
            return OK;
        case vr_U_U1_3_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_1);
            return OK;
        case vr_U_U1_3_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_2);
            return OK;
        case vr_U_U1_3_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_3);
            return OK;
        case vr_U_U1_3_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_4);
            return OK;
        case vr_U_U1_4_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_0);
            return OK;
        case vr_U_U1_4_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_1);
            return OK;
        case vr_U_U1_4_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_2);
            return OK;
        case vr_U_U1_4_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_3);
            return OK;
        case vr_U_U1_4_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_4);
            return OK;
        case vr_U_U1_5_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_0);
            return OK;
        case vr_U_U1_5_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_1);
            return OK;
        case vr_U_U1_5_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_2);
            return OK;
        case vr_U_U1_5_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_3);
            return OK;
        case vr_U_U1_5_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_4);
            return OK;
        case vr_U_U1_6_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_0);
            return OK;
        case vr_U_U1_6_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_1);
            return OK;
        case vr_U_U1_6_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_2);
            return OK;
        case vr_U_U1_6_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_3);
            return OK;
        case vr_U_U1_6_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_4);
            return OK;
        case vr_U_U1_7_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_0);
            return OK;
        case vr_U_U1_7_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_1);
            return OK;
        case vr_U_U1_7_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_2);
            return OK;
        case vr_U_U1_7_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_3);
            return OK;
        case vr_U_U1_7_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_4);
            return OK;
        case vr_U_U1_8_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_0);
            return OK;
        case vr_U_U1_8_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_1);
            return OK;
        case vr_U_U1_8_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_2);
            return OK;
        case vr_U_U1_8_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_3);
            return OK;
        case vr_U_U1_8_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_4);
            return OK;
#endif
    default:
        // Compose message for log with value reference
        logError(comp, "getFloat64: ValueReference %d not available.", vr);
        return Error;
    }
}

Status setFloat64(ModelInstance *comp, ValueReference vr, const double values[],
                  size_t nValues, size_t *index) {
    // Switch on the value reference
    switch (vr)
    {
        // Time is always a double value
        case vr_time:
            ASSERT_NVALUES(1);
            M(time) = values[(*index)++];
            return OK;
#if FMI_VERSION < 3
        // Inputs
        case vr_u_0_0:
            ASSERT_NVALUES(1);
            M(u_0_0) = values[(*index)++];
            return OK;
        case vr_u_0_1:
            ASSERT_NVALUES(1);
            M(u_0_1) = values[(*index)++];
            return OK;
        case vr_u_0_2:
            ASSERT_NVALUES(1);
            M(u_0_2) = values[(*index)++];
            return OK;
        case vr_u_0_3:
            ASSERT_NVALUES(1);
            M(u_0_3) = values[(*index)++];
            return OK;
        case vr_u_0_4:
            ASSERT_NVALUES(1);
            M(u_0_4) = values[(*index)++];
            return OK;
        // Locals
        case vr_X_X1_0_0:
            ASSERT_NVALUES(1);
            M(X_X1_0_0) = values[(*index)++];
            return OK;
        case vr_X_X1_0_1:
            ASSERT_NVALUES(1);
            M(X_X1_0_1) = values[(*index)++];
            return OK;
        case vr_X_X1_0_2:
            ASSERT_NVALUES(1);
            M(X_X1_0_2) = values[(*index)++];
            return OK;
        case vr_X_X1_1_0:
            ASSERT_NVALUES(1);
            M(X_X1_1_0) = values[(*index)++];
            return OK;
        case vr_X_X1_1_1:
            ASSERT_NVALUES(1);
            M(X_X1_1_1) = values[(*index)++];
            return OK;
        case vr_X_X1_1_2:
            ASSERT_NVALUES(1);
            M(X_X1_1_2) = values[(*index)++];
            return OK;
        case vr_X_X1_2_0:
            ASSERT_NVALUES(1);
            M(X_X1_2_0) = values[(*index)++];
            return OK;
        case vr_X_X1_2_1:
            ASSERT_NVALUES(1);
            M(X_X1_2_1) = values[(*index)++];
            return OK;
        case vr_X_X1_2_2:
            ASSERT_NVALUES(1);
            M(X_X1_2_2) = values[(*index)++];
            return OK;
        case vr_X_X1_3_0:
            ASSERT_NVALUES(1);
            M(X_X1_3_0) = values[(*index)++];
            return OK;
        case vr_X_X1_3_1:
            ASSERT_NVALUES(1);
            M(X_X1_3_1) = values[(*index)++];
            return OK;
        case vr_X_X1_3_2:
            ASSERT_NVALUES(1);
            M(X_X1_3_2) = values[(*index)++];
            return OK;
        case vr_X_X1_4_0:
            ASSERT_NVALUES(1);
            M(X_X1_4_0) = values[(*index)++];
            return OK;
        case vr_X_X1_4_1:
            ASSERT_NVALUES(1);
            M(X_X1_4_1) = values[(*index)++];
            return OK;
        case vr_X_X1_4_2:
            ASSERT_NVALUES(1);
            M(X_X1_4_2) = values[(*index)++];
            return OK;
        case vr_X_X1_5_0:
            ASSERT_NVALUES(1);
            M(X_X1_5_0) = values[(*index)++];
            return OK;
        case vr_X_X1_5_1:
            ASSERT_NVALUES(1);
            M(X_X1_5_1) = values[(*index)++];
            return OK;
        case vr_X_X1_5_2:
            ASSERT_NVALUES(1);
            M(X_X1_5_2) = values[(*index)++];
            return OK;
        case vr_X_X1_6_0:
            ASSERT_NVALUES(1);
            M(X_X1_6_0) = values[(*index)++];
            return OK;
        case vr_X_X1_6_1:
            ASSERT_NVALUES(1);
            M(X_X1_6_1) = values[(*index)++];
            return OK;
        case vr_X_X1_6_2:
            ASSERT_NVALUES(1);
            M(X_X1_6_2) = values[(*index)++];
            return OK;
        case vr_X_X1_7_0:
            ASSERT_NVALUES(1);
            M(X_X1_7_0) = values[(*index)++];
            return OK;
        case vr_X_X1_7_1:
            ASSERT_NVALUES(1);
            M(X_X1_7_1) = values[(*index)++];
            return OK;
        case vr_X_X1_7_2:
            ASSERT_NVALUES(1);
            M(X_X1_7_2) = values[(*index)++];
            return OK;
        case vr_X_X1_8_0:
            ASSERT_NVALUES(1);
            M(X_X1_8_0) = values[(*index)++];
            return OK;
        case vr_X_X1_8_1:
            ASSERT_NVALUES(1);
            M(X_X1_8_1) = values[(*index)++];
            return OK;
        case vr_X_X1_8_2:
            ASSERT_NVALUES(1);
            M(X_X1_8_2) = values[(*index)++];
            return OK;
        case vr_X_X1_9_0:
            ASSERT_NVALUES(1);
            M(X_X1_9_0) = values[(*index)++];
            return OK;
        case vr_X_X1_9_1:
            ASSERT_NVALUES(1);
            M(X_X1_9_1) = values[(*index)++];
            return OK;
        case vr_X_X1_9_2:
            ASSERT_NVALUES(1);
            M(X_X1_9_2) = values[(*index)++];
            return OK;
        case vr_U_U1_0_0:
            ASSERT_NVALUES(1);
            M(U_U1_0_0) = values[(*index)++];
            return OK;
        case vr_U_U1_0_1:
            ASSERT_NVALUES(1);
            M(U_U1_0_1) = values[(*index)++];
            return OK;
        case vr_U_U1_0_2:
            ASSERT_NVALUES(1);
            M(U_U1_0_2) = values[(*index)++];
            return OK;
        case vr_U_U1_0_3:
            ASSERT_NVALUES(1);
            M(U_U1_0_3) = values[(*index)++];
            return OK;
        case vr_U_U1_0_4:
            ASSERT_NVALUES(1);
            M(U_U1_0_4) = values[(*index)++];
            return OK;
        case vr_U_U1_1_0:
            ASSERT_NVALUES(1);
            M(U_U1_1_0) = values[(*index)++];
            return OK;
        case vr_U_U1_1_1:
            ASSERT_NVALUES(1);
            M(U_U1_1_1) = values[(*index)++];
            return OK;
        case vr_U_U1_1_2:
            ASSERT_NVALUES(1);
            M(U_U1_1_2) = values[(*index)++];
            return OK;
        case vr_U_U1_1_3:
            ASSERT_NVALUES(1);
            M(U_U1_1_3) = values[(*index)++];
            return OK;
        case vr_U_U1_1_4:
            ASSERT_NVALUES(1);
            M(U_U1_1_4) = values[(*index)++];
            return OK;
        case vr_U_U1_2_0:
            ASSERT_NVALUES(1);
            M(U_U1_2_0) = values[(*index)++];
            return OK;
        case vr_U_U1_2_1:
            ASSERT_NVALUES(1);
            M(U_U1_2_1) = values[(*index)++];
            return OK;
        case vr_U_U1_2_2:
            ASSERT_NVALUES(1);
            M(U_U1_2_2) = values[(*index)++];
            return OK;
        case vr_U_U1_2_3:
            ASSERT_NVALUES(1);
            M(U_U1_2_3) = values[(*index)++];
            return OK;
        case vr_U_U1_2_4:
            ASSERT_NVALUES(1);
            M(U_U1_2_4) = values[(*index)++];
            return OK;
        case vr_U_U1_3_0:
            ASSERT_NVALUES(1);
            M(U_U1_3_0) = values[(*index)++];
            return OK;
        case vr_U_U1_3_1:
            ASSERT_NVALUES(1);
            M(U_U1_3_1) = values[(*index)++];
            return OK;
        case vr_U_U1_3_2:
            ASSERT_NVALUES(1);
            M(U_U1_3_2) = values[(*index)++];
            return OK;
        case vr_U_U1_3_3:
            ASSERT_NVALUES(1);
            M(U_U1_3_3) = values[(*index)++];
            return OK;
        case vr_U_U1_3_4:
            ASSERT_NVALUES(1);
            M(U_U1_3_4) = values[(*index)++];
            return OK;
        case vr_U_U1_4_0:
            ASSERT_NVALUES(1);
            M(U_U1_4_0) = values[(*index)++];
            return OK;
        case vr_U_U1_4_1:
            ASSERT_NVALUES(1);
            M(U_U1_4_1) = values[(*index)++];
            return OK;
        case vr_U_U1_4_2:
            ASSERT_NVALUES(1);
            M(U_U1_4_2) = values[(*index)++];
            return OK;
        case vr_U_U1_4_3:
            ASSERT_NVALUES(1);
            M(U_U1_4_3) = values[(*index)++];
            return OK;
        case vr_U_U1_4_4:
            ASSERT_NVALUES(1);
            M(U_U1_4_4) = values[(*index)++];
            return OK;
        case vr_U_U1_5_0:
            ASSERT_NVALUES(1);
            M(U_U1_5_0) = values[(*index)++];
            return OK;
        case vr_U_U1_5_1:
            ASSERT_NVALUES(1);
            M(U_U1_5_1) = values[(*index)++];
            return OK;
        case vr_U_U1_5_2:
            ASSERT_NVALUES(1);
            M(U_U1_5_2) = values[(*index)++];
            return OK;
        case vr_U_U1_5_3:
            ASSERT_NVALUES(1);
            M(U_U1_5_3) = values[(*index)++];
            return OK;
        case vr_U_U1_5_4:
            ASSERT_NVALUES(1);
            M(U_U1_5_4) = values[(*index)++];
            return OK;
        case vr_U_U1_6_0:
            ASSERT_NVALUES(1);
            M(U_U1_6_0) = values[(*index)++];
            return OK;
        case vr_U_U1_6_1:
            ASSERT_NVALUES(1);
            M(U_U1_6_1) = values[(*index)++];
            return OK;
        case vr_U_U1_6_2:
            ASSERT_NVALUES(1);
            M(U_U1_6_2) = values[(*index)++];
            return OK;
        case vr_U_U1_6_3:
            ASSERT_NVALUES(1);
            M(U_U1_6_3) = values[(*index)++];
            return OK;
        case vr_U_U1_6_4:
            ASSERT_NVALUES(1);
            M(U_U1_6_4) = values[(*index)++];
            return OK;
        case vr_U_U1_7_0:
            ASSERT_NVALUES(1);
            M(U_U1_7_0) = values[(*index)++];
            return OK;
        case vr_U_U1_7_1:
            ASSERT_NVALUES(1);
            M(U_U1_7_1) = values[(*index)++];
            return OK;
        case vr_U_U1_7_2:
            ASSERT_NVALUES(1);
            M(U_U1_7_2) = values[(*index)++];
            return OK;
        case vr_U_U1_7_3:
            ASSERT_NVALUES(1);
            M(U_U1_7_3) = values[(*index)++];
            return OK;
        case vr_U_U1_7_4:
            ASSERT_NVALUES(1);
            M(U_U1_7_4) = values[(*index)++];
            return OK;
        case vr_U_U1_8_0:
            ASSERT_NVALUES(1);
            M(U_U1_8_0) = values[(*index)++];
            return OK;
        case vr_U_U1_8_1:
            ASSERT_NVALUES(1);
            M(U_U1_8_1) = values[(*index)++];
            return OK;
        case vr_U_U1_8_2:
            ASSERT_NVALUES(1);
            M(U_U1_8_2) = values[(*index)++];
            return OK;
        case vr_U_U1_8_3:
            ASSERT_NVALUES(1);
            M(U_U1_8_3) = values[(*index)++];
            return OK;
        case vr_U_U1_8_4:
            ASSERT_NVALUES(1);
            M(U_U1_8_4) = values[(*index)++];
            return OK;
#endif
    default:
        // Compose message for log with value reference
        logError(comp, "setFloat64: ValueReference %d not available.", vr);
        return Error;
    }
}

#if FMI_VERSION > 2
Status getFloat32(ModelInstance *comp, ValueReference vr, float values[],
                  size_t nValues, size_t *index) {

    switch (vr)
    {
        // Inputs
        case vr_u_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_0);
            return OK;
        case vr_u_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_1);
            return OK;
        case vr_u_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_2);
            return OK;
        case vr_u_0_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_3);
            return OK;
        case vr_u_0_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(u_0_4);
            return OK;
        // Outputs
        case vr_x_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_0_0);
            return OK;
        case vr_x_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_0_1);
            return OK;
        case vr_x_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_0_2);
            return OK;
        // Locals
        case vr_X_X1_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_0_0);
            return OK;
        case vr_X_X1_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_0_1);
            return OK;
        case vr_X_X1_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_0_2);
            return OK;
        case vr_X_X1_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_1_0);
            return OK;
        case vr_X_X1_1_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_1_1);
            return OK;
        case vr_X_X1_1_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_1_2);
            return OK;
        case vr_X_X1_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_2_0);
            return OK;
        case vr_X_X1_2_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_2_1);
            return OK;
        case vr_X_X1_2_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_2_2);
            return OK;
        case vr_X_X1_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_3_0);
            return OK;
        case vr_X_X1_3_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_3_1);
            return OK;
        case vr_X_X1_3_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_3_2);
            return OK;
        case vr_X_X1_4_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_4_0);
            return OK;
        case vr_X_X1_4_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_4_1);
            return OK;
        case vr_X_X1_4_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_4_2);
            return OK;
        case vr_X_X1_5_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_5_0);
            return OK;
        case vr_X_X1_5_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_5_1);
            return OK;
        case vr_X_X1_5_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_5_2);
            return OK;
        case vr_X_X1_6_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_6_0);
            return OK;
        case vr_X_X1_6_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_6_1);
            return OK;
        case vr_X_X1_6_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_6_2);
            return OK;
        case vr_X_X1_7_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_7_0);
            return OK;
        case vr_X_X1_7_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_7_1);
            return OK;
        case vr_X_X1_7_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_7_2);
            return OK;
        case vr_X_X1_8_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_8_0);
            return OK;
        case vr_X_X1_8_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_8_1);
            return OK;
        case vr_X_X1_8_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_8_2);
            return OK;
        case vr_X_X1_9_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_9_0);
            return OK;
        case vr_X_X1_9_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_9_1);
            return OK;
        case vr_X_X1_9_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(X_X1_9_2);
            return OK;
        case vr_U_U1_0_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_0);
            return OK;
        case vr_U_U1_0_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_1);
            return OK;
        case vr_U_U1_0_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_2);
            return OK;
        case vr_U_U1_0_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_3);
            return OK;
        case vr_U_U1_0_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_0_4);
            return OK;
        case vr_U_U1_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_0);
            return OK;
        case vr_U_U1_1_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_1);
            return OK;
        case vr_U_U1_1_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_2);
            return OK;
        case vr_U_U1_1_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_3);
            return OK;
        case vr_U_U1_1_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_1_4);
            return OK;
        case vr_U_U1_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_0);
            return OK;
        case vr_U_U1_2_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_1);
            return OK;
        case vr_U_U1_2_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_2);
            return OK;
        case vr_U_U1_2_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_3);
            return OK;
        case vr_U_U1_2_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_2_4);
            return OK;
        case vr_U_U1_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_0);
            return OK;
        case vr_U_U1_3_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_1);
            return OK;
        case vr_U_U1_3_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_2);
            return OK;
        case vr_U_U1_3_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_3);
            return OK;
        case vr_U_U1_3_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_3_4);
            return OK;
        case vr_U_U1_4_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_0);
            return OK;
        case vr_U_U1_4_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_1);
            return OK;
        case vr_U_U1_4_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_2);
            return OK;
        case vr_U_U1_4_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_3);
            return OK;
        case vr_U_U1_4_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_4_4);
            return OK;
        case vr_U_U1_5_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_0);
            return OK;
        case vr_U_U1_5_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_1);
            return OK;
        case vr_U_U1_5_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_2);
            return OK;
        case vr_U_U1_5_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_3);
            return OK;
        case vr_U_U1_5_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_5_4);
            return OK;
        case vr_U_U1_6_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_0);
            return OK;
        case vr_U_U1_6_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_1);
            return OK;
        case vr_U_U1_6_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_2);
            return OK;
        case vr_U_U1_6_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_3);
            return OK;
        case vr_U_U1_6_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_6_4);
            return OK;
        case vr_U_U1_7_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_0);
            return OK;
        case vr_U_U1_7_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_1);
            return OK;
        case vr_U_U1_7_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_2);
            return OK;
        case vr_U_U1_7_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_3);
            return OK;
        case vr_U_U1_7_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_7_4);
            return OK;
        case vr_U_U1_8_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_0);
            return OK;
        case vr_U_U1_8_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_1);
            return OK;
        case vr_U_U1_8_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_2);
            return OK;
        case vr_U_U1_8_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_3);
            return OK;
        case vr_U_U1_8_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(U_U1_8_4);
            return OK;
    default:
        // Compose message for log with value reference
        logError(comp, "getFloat32: ValueReference %d not available.", vr);
        return Error;
    }
}

Status setFloat32(ModelInstance *comp, ValueReference vr, const float values[],
                  size_t nValues, size_t *index) {
    // Switch on the value reference
    switch (vr)
    {
        // Inputs
        case vr_u_0_0:
            ASSERT_NVALUES(1);
            M(u_0_0) = values[(*index)++];
            return OK;
        case vr_u_0_1:
            ASSERT_NVALUES(1);
            M(u_0_1) = values[(*index)++];
            return OK;
        case vr_u_0_2:
            ASSERT_NVALUES(1);
            M(u_0_2) = values[(*index)++];
            return OK;
        case vr_u_0_3:
            ASSERT_NVALUES(1);
            M(u_0_3) = values[(*index)++];
            return OK;
        case vr_u_0_4:
            ASSERT_NVALUES(1);
            M(u_0_4) = values[(*index)++];
            return OK;
        // Locals
        case vr_X_X1_0_0:
            ASSERT_NVALUES(1);
            M(X_X1_0_0) = values[(*index)++];
            return OK;
        case vr_X_X1_0_1:
            ASSERT_NVALUES(1);
            M(X_X1_0_1) = values[(*index)++];
            return OK;
        case vr_X_X1_0_2:
            ASSERT_NVALUES(1);
            M(X_X1_0_2) = values[(*index)++];
            return OK;
        case vr_X_X1_1_0:
            ASSERT_NVALUES(1);
            M(X_X1_1_0) = values[(*index)++];
            return OK;
        case vr_X_X1_1_1:
            ASSERT_NVALUES(1);
            M(X_X1_1_1) = values[(*index)++];
            return OK;
        case vr_X_X1_1_2:
            ASSERT_NVALUES(1);
            M(X_X1_1_2) = values[(*index)++];
            return OK;
        case vr_X_X1_2_0:
            ASSERT_NVALUES(1);
            M(X_X1_2_0) = values[(*index)++];
            return OK;
        case vr_X_X1_2_1:
            ASSERT_NVALUES(1);
            M(X_X1_2_1) = values[(*index)++];
            return OK;
        case vr_X_X1_2_2:
            ASSERT_NVALUES(1);
            M(X_X1_2_2) = values[(*index)++];
            return OK;
        case vr_X_X1_3_0:
            ASSERT_NVALUES(1);
            M(X_X1_3_0) = values[(*index)++];
            return OK;
        case vr_X_X1_3_1:
            ASSERT_NVALUES(1);
            M(X_X1_3_1) = values[(*index)++];
            return OK;
        case vr_X_X1_3_2:
            ASSERT_NVALUES(1);
            M(X_X1_3_2) = values[(*index)++];
            return OK;
        case vr_X_X1_4_0:
            ASSERT_NVALUES(1);
            M(X_X1_4_0) = values[(*index)++];
            return OK;
        case vr_X_X1_4_1:
            ASSERT_NVALUES(1);
            M(X_X1_4_1) = values[(*index)++];
            return OK;
        case vr_X_X1_4_2:
            ASSERT_NVALUES(1);
            M(X_X1_4_2) = values[(*index)++];
            return OK;
        case vr_X_X1_5_0:
            ASSERT_NVALUES(1);
            M(X_X1_5_0) = values[(*index)++];
            return OK;
        case vr_X_X1_5_1:
            ASSERT_NVALUES(1);
            M(X_X1_5_1) = values[(*index)++];
            return OK;
        case vr_X_X1_5_2:
            ASSERT_NVALUES(1);
            M(X_X1_5_2) = values[(*index)++];
            return OK;
        case vr_X_X1_6_0:
            ASSERT_NVALUES(1);
            M(X_X1_6_0) = values[(*index)++];
            return OK;
        case vr_X_X1_6_1:
            ASSERT_NVALUES(1);
            M(X_X1_6_1) = values[(*index)++];
            return OK;
        case vr_X_X1_6_2:
            ASSERT_NVALUES(1);
            M(X_X1_6_2) = values[(*index)++];
            return OK;
        case vr_X_X1_7_0:
            ASSERT_NVALUES(1);
            M(X_X1_7_0) = values[(*index)++];
            return OK;
        case vr_X_X1_7_1:
            ASSERT_NVALUES(1);
            M(X_X1_7_1) = values[(*index)++];
            return OK;
        case vr_X_X1_7_2:
            ASSERT_NVALUES(1);
            M(X_X1_7_2) = values[(*index)++];
            return OK;
        case vr_X_X1_8_0:
            ASSERT_NVALUES(1);
            M(X_X1_8_0) = values[(*index)++];
            return OK;
        case vr_X_X1_8_1:
            ASSERT_NVALUES(1);
            M(X_X1_8_1) = values[(*index)++];
            return OK;
        case vr_X_X1_8_2:
            ASSERT_NVALUES(1);
            M(X_X1_8_2) = values[(*index)++];
            return OK;
        case vr_X_X1_9_0:
            ASSERT_NVALUES(1);
            M(X_X1_9_0) = values[(*index)++];
            return OK;
        case vr_X_X1_9_1:
            ASSERT_NVALUES(1);
            M(X_X1_9_1) = values[(*index)++];
            return OK;
        case vr_X_X1_9_2:
            ASSERT_NVALUES(1);
            M(X_X1_9_2) = values[(*index)++];
            return OK;
        case vr_U_U1_0_0:
            ASSERT_NVALUES(1);
            M(U_U1_0_0) = values[(*index)++];
            return OK;
        case vr_U_U1_0_1:
            ASSERT_NVALUES(1);
            M(U_U1_0_1) = values[(*index)++];
            return OK;
        case vr_U_U1_0_2:
            ASSERT_NVALUES(1);
            M(U_U1_0_2) = values[(*index)++];
            return OK;
        case vr_U_U1_0_3:
            ASSERT_NVALUES(1);
            M(U_U1_0_3) = values[(*index)++];
            return OK;
        case vr_U_U1_0_4:
            ASSERT_NVALUES(1);
            M(U_U1_0_4) = values[(*index)++];
            return OK;
        case vr_U_U1_1_0:
            ASSERT_NVALUES(1);
            M(U_U1_1_0) = values[(*index)++];
            return OK;
        case vr_U_U1_1_1:
            ASSERT_NVALUES(1);
            M(U_U1_1_1) = values[(*index)++];
            return OK;
        case vr_U_U1_1_2:
            ASSERT_NVALUES(1);
            M(U_U1_1_2) = values[(*index)++];
            return OK;
        case vr_U_U1_1_3:
            ASSERT_NVALUES(1);
            M(U_U1_1_3) = values[(*index)++];
            return OK;
        case vr_U_U1_1_4:
            ASSERT_NVALUES(1);
            M(U_U1_1_4) = values[(*index)++];
            return OK;
        case vr_U_U1_2_0:
            ASSERT_NVALUES(1);
            M(U_U1_2_0) = values[(*index)++];
            return OK;
        case vr_U_U1_2_1:
            ASSERT_NVALUES(1);
            M(U_U1_2_1) = values[(*index)++];
            return OK;
        case vr_U_U1_2_2:
            ASSERT_NVALUES(1);
            M(U_U1_2_2) = values[(*index)++];
            return OK;
        case vr_U_U1_2_3:
            ASSERT_NVALUES(1);
            M(U_U1_2_3) = values[(*index)++];
            return OK;
        case vr_U_U1_2_4:
            ASSERT_NVALUES(1);
            M(U_U1_2_4) = values[(*index)++];
            return OK;
        case vr_U_U1_3_0:
            ASSERT_NVALUES(1);
            M(U_U1_3_0) = values[(*index)++];
            return OK;
        case vr_U_U1_3_1:
            ASSERT_NVALUES(1);
            M(U_U1_3_1) = values[(*index)++];
            return OK;
        case vr_U_U1_3_2:
            ASSERT_NVALUES(1);
            M(U_U1_3_2) = values[(*index)++];
            return OK;
        case vr_U_U1_3_3:
            ASSERT_NVALUES(1);
            M(U_U1_3_3) = values[(*index)++];
            return OK;
        case vr_U_U1_3_4:
            ASSERT_NVALUES(1);
            M(U_U1_3_4) = values[(*index)++];
            return OK;
        case vr_U_U1_4_0:
            ASSERT_NVALUES(1);
            M(U_U1_4_0) = values[(*index)++];
            return OK;
        case vr_U_U1_4_1:
            ASSERT_NVALUES(1);
            M(U_U1_4_1) = values[(*index)++];
            return OK;
        case vr_U_U1_4_2:
            ASSERT_NVALUES(1);
            M(U_U1_4_2) = values[(*index)++];
            return OK;
        case vr_U_U1_4_3:
            ASSERT_NVALUES(1);
            M(U_U1_4_3) = values[(*index)++];
            return OK;
        case vr_U_U1_4_4:
            ASSERT_NVALUES(1);
            M(U_U1_4_4) = values[(*index)++];
            return OK;
        case vr_U_U1_5_0:
            ASSERT_NVALUES(1);
            M(U_U1_5_0) = values[(*index)++];
            return OK;
        case vr_U_U1_5_1:
            ASSERT_NVALUES(1);
            M(U_U1_5_1) = values[(*index)++];
            return OK;
        case vr_U_U1_5_2:
            ASSERT_NVALUES(1);
            M(U_U1_5_2) = values[(*index)++];
            return OK;
        case vr_U_U1_5_3:
            ASSERT_NVALUES(1);
            M(U_U1_5_3) = values[(*index)++];
            return OK;
        case vr_U_U1_5_4:
            ASSERT_NVALUES(1);
            M(U_U1_5_4) = values[(*index)++];
            return OK;
        case vr_U_U1_6_0:
            ASSERT_NVALUES(1);
            M(U_U1_6_0) = values[(*index)++];
            return OK;
        case vr_U_U1_6_1:
            ASSERT_NVALUES(1);
            M(U_U1_6_1) = values[(*index)++];
            return OK;
        case vr_U_U1_6_2:
            ASSERT_NVALUES(1);
            M(U_U1_6_2) = values[(*index)++];
            return OK;
        case vr_U_U1_6_3:
            ASSERT_NVALUES(1);
            M(U_U1_6_3) = values[(*index)++];
            return OK;
        case vr_U_U1_6_4:
            ASSERT_NVALUES(1);
            M(U_U1_6_4) = values[(*index)++];
            return OK;
        case vr_U_U1_7_0:
            ASSERT_NVALUES(1);
            M(U_U1_7_0) = values[(*index)++];
            return OK;
        case vr_U_U1_7_1:
            ASSERT_NVALUES(1);
            M(U_U1_7_1) = values[(*index)++];
            return OK;
        case vr_U_U1_7_2:
            ASSERT_NVALUES(1);
            M(U_U1_7_2) = values[(*index)++];
            return OK;
        case vr_U_U1_7_3:
            ASSERT_NVALUES(1);
            M(U_U1_7_3) = values[(*index)++];
            return OK;
        case vr_U_U1_7_4:
            ASSERT_NVALUES(1);
            M(U_U1_7_4) = values[(*index)++];
            return OK;
        case vr_U_U1_8_0:
            ASSERT_NVALUES(1);
            M(U_U1_8_0) = values[(*index)++];
            return OK;
        case vr_U_U1_8_1:
            ASSERT_NVALUES(1);
            M(U_U1_8_1) = values[(*index)++];
            return OK;
        case vr_U_U1_8_2:
            ASSERT_NVALUES(1);
            M(U_U1_8_2) = values[(*index)++];
            return OK;
        case vr_U_U1_8_3:
            ASSERT_NVALUES(1);
            M(U_U1_8_3) = values[(*index)++];
            return OK;
        case vr_U_U1_8_4:
            ASSERT_NVALUES(1);
            M(U_U1_8_4) = values[(*index)++];
            return OK;
    default:
        // Compose message for log with value reference
        logError(comp, "setFloat32: ValueReference %d not available.", vr);
        return Error;
    }
}
#endif

Status eventUpdate(ModelInstance *comp) {

    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;

    return OK;
}
