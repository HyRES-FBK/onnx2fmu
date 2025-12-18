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
    M(x_1_0) = (double) 1.0;
    M(x_2_0) = (double) 1.0;
    M(x_3_0) = (double) 1.0;
    // Local variables
}

Status calculateValues(ModelInstance *comp) {

    // Do I need memory info?
    OrtMemoryInfo* memory_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateCpuMemoryInfo(
            OrtArenaAllocator, OrtMemTypeDefault, &memory_info
        ),
        comp);

    // Create x_1 tensor
    OrtValue* x_1_tensor;

    // Store the shape of the input tensor
    const size_t x_1_shape[] = { 1 };

    // Determine the dimensions of the input tensor
    const size_t x_1_dim = sizeof(x_1_shape) / sizeof(x_1_shape[0]);
    size_t x_1_size = 1;
    for (size_t i = 0; i < x_1_dim; ++i) {
        x_1_size *= x_1_shape[i];
    }

    // Store values in the flattened array
    float* x_1_float = (float*)malloc(x_1_size * sizeof(float));
    if (x_1_float == NULL) {
        logError(comp, "Failed to allocate memory for x_1_float");
        return Error;
    }

    // Flatten the input array
    x_1_float[0] = (float)M(x_1_0);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            x_1_float,
            x_1_size * sizeof(float),
            (const int64_t*)x_1_shape,
            x_1_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &x_1_tensor
        ),
        comp);

    int x_1_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            x_1_tensor,
            &x_1_is_tensor),
            comp
        );

    assert(x_1_is_tensor);

    // Create x_2 tensor
    OrtValue* x_2_tensor;

    // Store the shape of the input tensor
    const size_t x_2_shape[] = { 1 };

    // Determine the dimensions of the input tensor
    const size_t x_2_dim = sizeof(x_2_shape) / sizeof(x_2_shape[0]);
    size_t x_2_size = 1;
    for (size_t i = 0; i < x_2_dim; ++i) {
        x_2_size *= x_2_shape[i];
    }

    // Store values in the flattened array
    float* x_2_float = (float*)malloc(x_2_size * sizeof(float));
    if (x_2_float == NULL) {
        logError(comp, "Failed to allocate memory for x_2_float");
        return Error;
    }

    // Flatten the input array
    x_2_float[0] = (float)M(x_2_0);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            x_2_float,
            x_2_size * sizeof(float),
            (const int64_t*)x_2_shape,
            x_2_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &x_2_tensor
        ),
        comp);

    int x_2_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            x_2_tensor,
            &x_2_is_tensor),
            comp
        );

    assert(x_2_is_tensor);

    // Create x_3 tensor
    OrtValue* x_3_tensor;

    // Store the shape of the input tensor
    const size_t x_3_shape[] = { 1 };

    // Determine the dimensions of the input tensor
    const size_t x_3_dim = sizeof(x_3_shape) / sizeof(x_3_shape[0]);
    size_t x_3_size = 1;
    for (size_t i = 0; i < x_3_dim; ++i) {
        x_3_size *= x_3_shape[i];
    }

    // Store values in the flattened array
    float* x_3_float = (float*)malloc(x_3_size * sizeof(float));
    if (x_3_float == NULL) {
        logError(comp, "Failed to allocate memory for x_3_float");
        return Error;
    }

    // Flatten the input array
    x_3_float[0] = (float)M(x_3_0);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            x_3_float,
            x_3_size * sizeof(float),
            (const int64_t*)x_3_shape,
            x_3_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &x_3_tensor
        ),
        comp);

    int x_3_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            x_3_tensor,
            &x_3_is_tensor),
            comp
        );

    assert(x_3_is_tensor);

    // LOCAL variables

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Create output tensors
    OrtValue* y_tensor = NULL;
    // Create output tensors for local variables


    // Declare input node names
    const char* input_names[] = {
        "x_1",
        "x_2",
        "x_3"
    };

    // Declare output node names
    const char* output_names[] = {
        "y"
    };

    // Gather input tensors
    const OrtValue* input_tensors[] = {
        x_1_tensor,
        x_2_tensor,
        x_3_tensor
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        y_tensor
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
            1,
            output_tensors
        ),
        comp
    );

    // Check output tensors to be tensors
    int y_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[0],
            &y_is_tensor),
            comp
        );
    assert (y_is_tensor);

    // Check local output tensors to be tensors

    // Retrieve pointer to the y tensor
    float* y_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[0],
            (void**)&y_tensor_data
        ),
        comp
    );

    // Retrieve y tensor info
    OrtTensorTypeAndShapeInfo* y_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[0],
            &y_info
        ),
        comp
    );

    // Retrieve y tensor shape
    size_t y_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            y_info,
            &y_dims
        ),
        comp
    );

    // Set y tensor data to model
    M(y_0) = y_tensor_data[0];
    M(y_1) = y_tensor_data[1];

    // Free tensors
    comp->g_ort->ReleaseValue(y_tensor);
    comp->g_ort->ReleaseValue(x_1_tensor);
    comp->g_ort->ReleaseValue(x_2_tensor);
    comp->g_ort->ReleaseValue(x_3_tensor);

    // Free arrays
    free(x_1_float);
    free(x_2_float);
    free(x_3_float);

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
        case vr_x_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_1_0);
            return OK;
        case vr_x_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_2_0);
            return OK;
        case vr_x_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_3_0);
            return OK;
        // Outputs
        case vr_y_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(y_0);
            return OK;
        case vr_y_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(y_1);
            return OK;
        // Locals
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
        case vr_x_1_0:
            ASSERT_NVALUES(1);
            M(x_1_0) = values[(*index)++];
            return OK;
        case vr_x_2_0:
            ASSERT_NVALUES(1);
            M(x_2_0) = values[(*index)++];
            return OK;
        case vr_x_3_0:
            ASSERT_NVALUES(1);
            M(x_3_0) = values[(*index)++];
            return OK;
        // Locals
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
        case vr_x_1_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_1_0);
            return OK;
        case vr_x_2_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_2_0);
            return OK;
        case vr_x_3_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(x_3_0);
            return OK;
        // Outputs
        case vr_y_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(y_0);
            return OK;
        case vr_y_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(y_1);
            return OK;
        // Locals
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
        case vr_x_1_0:
            ASSERT_NVALUES(1);
            M(x_1_0) = values[(*index)++];
            return OK;
        case vr_x_2_0:
            ASSERT_NVALUES(1);
            M(x_2_0) = values[(*index)++];
            return OK;
        case vr_x_3_0:
            ASSERT_NVALUES(1);
            M(x_3_0) = values[(*index)++];
            return OK;
        // Locals
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
