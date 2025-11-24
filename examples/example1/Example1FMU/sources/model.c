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
    M(scalar_input_0) = (double) 1.0;
    M(vector_input_0) = (double) 1.0;
    M(vector_input_1) = (double) 1.0;
    M(vector_input_2) = (double) 1.0;
    M(vector_input_3) = (double) 1.0;
    M(vector_input_discrete_0) = (double) 1.0;
    M(vector_input_discrete_1) = (double) 1.0;
    M(vector_input_discrete_2) = (double) 1.0;
    M(vector_input_discrete_3) = (double) 1.0;
    M(vector_input_discrete_4) = (double) 1.0;
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

    // Create scalar_input tensor
    OrtValue* scalar_input_tensor;

    // Store the shape of the input tensor
    const size_t scalar_input_shape[] = { 1 };

    // Determine the dimensions of the input tensor
    const size_t scalar_input_dim = sizeof(scalar_input_shape) / sizeof(scalar_input_shape[0]);
    size_t scalar_input_size = 1;
    for (size_t i = 0; i < scalar_input_dim; ++i) {
        scalar_input_size *= scalar_input_shape[i];
    }

    // Store values in the flattened array
    float* scalar_input_float = (float*)malloc(scalar_input_size * sizeof(float));
    if (scalar_input_float == NULL) {
        logError(comp, "Failed to allocate memory for scalar_input_float");
        return Error;
    }

    // Flatten the input array
    scalar_input_float[0] = (float)M(scalar_input_0);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            scalar_input_float,
            scalar_input_size * sizeof(float),
            (const int64_t*)scalar_input_shape,
            scalar_input_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &scalar_input_tensor
        ),
        comp);

    int scalar_input_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            scalar_input_tensor,
            &scalar_input_is_tensor),
            comp
        );

    assert(scalar_input_is_tensor);

    // Create vector_input tensor
    OrtValue* vector_input_tensor;

    // Store the shape of the input tensor
    const size_t vector_input_shape[] = { 4 };

    // Determine the dimensions of the input tensor
    const size_t vector_input_dim = sizeof(vector_input_shape) / sizeof(vector_input_shape[0]);
    size_t vector_input_size = 1;
    for (size_t i = 0; i < vector_input_dim; ++i) {
        vector_input_size *= vector_input_shape[i];
    }

    // Store values in the flattened array
    float* vector_input_float = (float*)malloc(vector_input_size * sizeof(float));
    if (vector_input_float == NULL) {
        logError(comp, "Failed to allocate memory for vector_input_float");
        return Error;
    }

    // Flatten the input array
    vector_input_float[0] = (float)M(vector_input_0);
    vector_input_float[1] = (float)M(vector_input_1);
    vector_input_float[2] = (float)M(vector_input_2);
    vector_input_float[3] = (float)M(vector_input_3);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            vector_input_float,
            vector_input_size * sizeof(float),
            (const int64_t*)vector_input_shape,
            vector_input_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &vector_input_tensor
        ),
        comp);

    int vector_input_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            vector_input_tensor,
            &vector_input_is_tensor),
            comp
        );

    assert(vector_input_is_tensor);

    // Create vector_input_discrete tensor
    OrtValue* vector_input_discrete_tensor;

    // Store the shape of the input tensor
    const size_t vector_input_discrete_shape[] = { 5 };

    // Determine the dimensions of the input tensor
    const size_t vector_input_discrete_dim = sizeof(vector_input_discrete_shape) / sizeof(vector_input_discrete_shape[0]);
    size_t vector_input_discrete_size = 1;
    for (size_t i = 0; i < vector_input_discrete_dim; ++i) {
        vector_input_discrete_size *= vector_input_discrete_shape[i];
    }

    // Store values in the flattened array
    float* vector_input_discrete_float = (float*)malloc(vector_input_discrete_size * sizeof(float));
    if (vector_input_discrete_float == NULL) {
        logError(comp, "Failed to allocate memory for vector_input_discrete_float");
        return Error;
    }

    // Flatten the input array
    vector_input_discrete_float[0] = (float)M(vector_input_discrete_0);
    vector_input_discrete_float[1] = (float)M(vector_input_discrete_1);
    vector_input_discrete_float[2] = (float)M(vector_input_discrete_2);
    vector_input_discrete_float[3] = (float)M(vector_input_discrete_3);
    vector_input_discrete_float[4] = (float)M(vector_input_discrete_4);

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            vector_input_discrete_float,
            vector_input_discrete_size * sizeof(float),
            (const int64_t*)vector_input_discrete_shape,
            vector_input_discrete_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &vector_input_discrete_tensor
        ),
        comp);

    int vector_input_discrete_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            vector_input_discrete_tensor,
            &vector_input_discrete_is_tensor),
            comp
        );

    assert(vector_input_discrete_is_tensor);

    // LOCAL variables

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Create output tensors
    OrtValue* output_tensor = NULL;
    // Create output tensors for local variables


    // Declare input node names
    const char* input_names[] = {
        "scalar_input",
        "vector_input",
        "vector_input_discrete"
    };

    // Declare output node names
    const char* output_names[] = {
        "output"
    };

    // Gather input tensors
    const OrtValue* input_tensors[] = {
        scalar_input_tensor,
        vector_input_tensor,
        vector_input_discrete_tensor
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        output_tensor
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
    int output_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[0],
            &output_is_tensor),
            comp
        );
    assert (output_is_tensor);

    // Check local output tensors to be tensors

    // Retrieve pointer to the output tensor
    float* output_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[0],
            (void**)&output_tensor_data
        ),
        comp
    );

    // Retrieve output tensor info
    OrtTensorTypeAndShapeInfo* output_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[0],
            &output_info
        ),
        comp
    );

    // Retrieve output tensor shape
    size_t output_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            output_info,
            &output_dims
        ),
        comp
    );

    // Set output tensor data to model
    M(output_0) = output_tensor_data[0];
    M(output_1) = output_tensor_data[1];
    M(output_2) = output_tensor_data[2];
    M(output_3) = output_tensor_data[3];
    M(output_4) = output_tensor_data[4];
    M(output_5) = output_tensor_data[5];
    M(output_6) = output_tensor_data[6];
    M(output_7) = output_tensor_data[7];
    M(output_8) = output_tensor_data[8];
    M(output_9) = output_tensor_data[9];

    // Free tensors
    comp->g_ort->ReleaseValue(output_tensor);
    comp->g_ort->ReleaseValue(scalar_input_tensor);
    comp->g_ort->ReleaseValue(vector_input_tensor);
    comp->g_ort->ReleaseValue(vector_input_discrete_tensor);

    // Free arrays
    free(scalar_input_float);
    free(vector_input_float);
    free(vector_input_discrete_float);

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
        case vr_scalar_input_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(scalar_input_0);
            return OK;
        case vr_vector_input_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_0);
            return OK;
        case vr_vector_input_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_1);
            return OK;
        case vr_vector_input_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_2);
            return OK;
        case vr_vector_input_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_3);
            return OK;
        case vr_vector_input_discrete_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_0);
            return OK;
        case vr_vector_input_discrete_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_1);
            return OK;
        case vr_vector_input_discrete_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_2);
            return OK;
        case vr_vector_input_discrete_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_3);
            return OK;
        case vr_vector_input_discrete_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_4);
            return OK;
        // Outputs
        case vr_output_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_0);
            return OK;
        case vr_output_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_1);
            return OK;
        case vr_output_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_2);
            return OK;
        case vr_output_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_3);
            return OK;
        case vr_output_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_4);
            return OK;
        case vr_output_5:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_5);
            return OK;
        case vr_output_6:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_6);
            return OK;
        case vr_output_7:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_7);
            return OK;
        case vr_output_8:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_8);
            return OK;
        case vr_output_9:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_9);
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
        case vr_scalar_input_0:
            ASSERT_NVALUES(1);
            M(scalar_input_0) = values[(*index)++];
            return OK;
        case vr_vector_input_0:
            ASSERT_NVALUES(1);
            M(vector_input_0) = values[(*index)++];
            return OK;
        case vr_vector_input_1:
            ASSERT_NVALUES(1);
            M(vector_input_1) = values[(*index)++];
            return OK;
        case vr_vector_input_2:
            ASSERT_NVALUES(1);
            M(vector_input_2) = values[(*index)++];
            return OK;
        case vr_vector_input_3:
            ASSERT_NVALUES(1);
            M(vector_input_3) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_0:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_0) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_1:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_1) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_2:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_2) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_3:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_3) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_4:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_4) = values[(*index)++];
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
        case vr_scalar_input_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(scalar_input_0);
            return OK;
        case vr_vector_input_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_0);
            return OK;
        case vr_vector_input_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_1);
            return OK;
        case vr_vector_input_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_2);
            return OK;
        case vr_vector_input_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_3);
            return OK;
        case vr_vector_input_discrete_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_0);
            return OK;
        case vr_vector_input_discrete_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_1);
            return OK;
        case vr_vector_input_discrete_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_2);
            return OK;
        case vr_vector_input_discrete_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_3);
            return OK;
        case vr_vector_input_discrete_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(vector_input_discrete_4);
            return OK;
        // Outputs
        case vr_output_0:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_0);
            return OK;
        case vr_output_1:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_1);
            return OK;
        case vr_output_2:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_2);
            return OK;
        case vr_output_3:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_3);
            return OK;
        case vr_output_4:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_4);
            return OK;
        case vr_output_5:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_5);
            return OK;
        case vr_output_6:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_6);
            return OK;
        case vr_output_7:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_7);
            return OK;
        case vr_output_8:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_8);
            return OK;
        case vr_output_9:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(output_9);
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
        case vr_scalar_input_0:
            ASSERT_NVALUES(1);
            M(scalar_input_0) = values[(*index)++];
            return OK;
        case vr_vector_input_0:
            ASSERT_NVALUES(1);
            M(vector_input_0) = values[(*index)++];
            return OK;
        case vr_vector_input_1:
            ASSERT_NVALUES(1);
            M(vector_input_1) = values[(*index)++];
            return OK;
        case vr_vector_input_2:
            ASSERT_NVALUES(1);
            M(vector_input_2) = values[(*index)++];
            return OK;
        case vr_vector_input_3:
            ASSERT_NVALUES(1);
            M(vector_input_3) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_0:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_0) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_1:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_1) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_2:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_2) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_3:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_3) = values[(*index)++];
            return OK;
        case vr_vector_input_discrete_4:
            ASSERT_NVALUES(1);
            M(vector_input_discrete_4) = values[(*index)++];
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
