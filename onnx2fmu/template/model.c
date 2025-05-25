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
    UNUSED(comp);
}

Status calculateValues(ModelInstance *comp) {

    // Do I need memory info?
    OrtMemoryInfo* memory_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateCpuMemoryInfo(
            OrtArenaAllocator, OrtMemTypeDefault, &memory_info
        ),
        comp);
    {%- for input in inputs %}

    // Create {{ input.name }} tensor
    OrtValue* {{ input.name }}_tensor;

    // Store the shape of the input tensor
    const size_t {{ input.name }}_shape[] = { {{ input.shape|join(", ") }} };

    // Determine the dimensions of the input tensor
    const size_t {{ input.name }}_dim = sizeof({{ input.name }}_shape) / sizeof({{ input.name }}_shape[0]);
    size_t {{ input.name }}_size = 1;
    for (size_t i = 0; i < {{ input.name }}_dim; ++i) {
        {{ input.name }}_size *= {{ input.name }}_shape[i];
    }

    // Store values in the flattened array
    float* {{ input.name }}_float = (float*)malloc({{ input.name }}_size * sizeof(float));
    if ({{ input.name }}_float == NULL) {
        logError(comp, "Failed to allocate memory for {{ input.name }}_float");
        return Error;
    }

    // Flatten the input array
    {%- for scalar in input.scalarValues %}
    {{ input.name }}_float[{{ loop.index0 }}] = (float)M({{ scalar.name }});
    {%- endfor %}

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            {{ input.name }}_float,
            {{ input.name }}_size * sizeof(float),
            (const int64_t*){{ input.name }}_shape,
            {{ input.name }}_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &{{ input.name }}_tensor
        ),
        comp);

    int {{ input.name }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            {{ input.name }}_tensor,
            &{{ input.name }}_is_tensor),
            comp
        );

    assert({{ input.name }}_is_tensor);
    {%- endfor %}

    // LOCAL variables
    {%- for local in locals %}

    // Create {{ local.nameIn }} tensor
    OrtValue* {{ local.nameIn }}_tensor;

    // Store the shape of the input tensor
    const size_t {{ local.nameIn }}_shape[] = { {{ local.shape|join(", ") }} };

    // Determine the dimensions of the local tensor
    const size_t {{ local.nameIn }}_dim = sizeof({{ local.nameIn }}_shape) / sizeof({{ local.nameIn }}_shape[0]);
    size_t {{ local.nameIn }}_size = 1;
    for (size_t i = 0; i < {{ local.nameIn }}_dim; ++i) {
        {{ local.nameIn }}_size *= {{ local.nameIn }}_shape[i];
    }

    // Store values in the flattened array
    float* {{ local.nameIn }}_float = (float*)malloc({{ local.nameIn }}_size * sizeof(float));
    if ({{ local.nameIn }}_float == NULL) {
        logError(comp, "Failed to allocate memory for {{ local.nameIn }}_float");
        return Error;
    }

    // Flatten the local array
    {%- for scalar in local.scalarValues %}
    {{ local.nameIn }}_float[{{ loop.index0 }}] = (float)M({{ scalar.name }});
    {%- endfor %}

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            {{ local.nameIn }}_float,
            {{ local.nameIn }}_size * sizeof(float),
            (const int64_t*){{ local.nameIn }}_shape,
            {{ local.nameIn }}_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &{{ local.nameIn }}_tensor
        ),
        comp);

    int {{ local.nameIn }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            {{ local.nameIn }}_tensor,
            &{{ local.nameIn }}_is_tensor),
            comp
        );

    assert({{ local.nameIn }}_is_tensor);
    {%- endfor %}

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Create output tensors
    {%- for output in outputs %}
    OrtValue* {{ output.name }}_tensor = NULL;
    {%- endfor %}
    // Create output tensors for local variables
    {%- for local in locals %}
    OrtValue* {{ local.nameOut }}_tensor = NULL;
    {%- endfor %}


    // Declare input node names
    const char* input_names[] = {
        {%- for input in inputs %}
        "{{ input.name }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if locals %},{% endif %}
        {%- for local in locals %}
        "{{ local.nameIn }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Declare output node names
    const char* output_names[] = {
        {%- for output in outputs %}
        "{{ output.name }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if locals %},{% endif %}
        {%- for local in locals %}
        "{{ local.nameOut }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Gather input tensors
    const OrtValue* input_tensors[] = {
        {%- for input in inputs %}
        {{ input.name }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if locals %},{% endif %}
        {%- for local in locals %}
        {{ local.nameIn }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        {%- for output in outputs %}
        {{ output.name }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if locals %},{% endif %}
        {%- for local in locals %}
        {{ local.nameOut }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Run inference
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(
            comp->session,
            NULL,
            input_names,
            input_tensors,
            {{ inputs|length + locals|length }},
            output_names,
            {{ outputs|length + locals|length }},
            output_tensors
        ),
        comp
    );

    // Check output tensors to be tensors
    {%- for output in outputs %}
    int {{ output.name }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[{{ loop.index0 }}],
            &{{ output.name }}_is_tensor),
            comp
        );
    assert ({{ output.name }}_is_tensor);
    {%- endfor %}

    // Check local output tensors to be tensors
    {%- for local in locals %}
    int {{ local.nameOut }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[{{ loop.index0 + outputs|length }}],
            &{{ local.nameOut }}_is_tensor),
            comp
        );
    assert ({{ local.nameOut }}_is_tensor);

    {%- endfor %}

    {%- for output in outputs %}

    // Retrieve pointer to the {{ output.name }} tensor
    float* {{ output.name }}_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[{{ loop.index0 }}],
            (void**)&{{ output.name }}_tensor_data
        ),
        comp
    );

    // Retrieve {{ output.name }} tensor info
    OrtTensorTypeAndShapeInfo* {{ output.name }}_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[{{ loop.index0 }}],
            &{{ output.name }}_info
        ),
        comp
    );

    // Retrieve {{ output.name }} tensor shape
    size_t {{ output.name }}_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            {{ output.name }}_info,
            &{{ output.name }}_dims
        ),
        comp
    );

    // Set {{ output.name }} tensor data to model
    {%- for scalar in output.scalarValues %}
    M({{ scalar.name }}) = {{ output.name }}_tensor_data[{{ loop.index0 }}];
    {%- endfor %}
    {%- endfor %}

    {%- for local in locals %}

    // Retrieve pointer to the {{ local.nameOut }} tensor
    float* {{ local.nameOut }}_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[{{ loop.index0 + outputs|length }}],
            (void**)&{{ local.nameOut }}_tensor_data
        ),
        comp
    );

    // Retrieve {{ local.nameOut }} tensor info
    OrtTensorTypeAndShapeInfo* {{ local.nameOut }}_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[{{ loop.index0 + outputs|length }}],
            &{{ local.nameOut }}_info
        ),
        comp
    );

    // Retrieve {{ local.name }} tensor shape
    size_t {{ local.nameOut }}_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            {{ local.nameOut }}_info,
            &{{ local.nameOut }}_dims
        ),
        comp
    );

    // Set {{ local.nameOut }} tensor data to model
    {%- for scalar in local.scalarValues %}
    M({{ scalar.name }}) = {{ local.nameOut }}_tensor_data[{{ loop.index0 }}];
    {%- endfor %}
    {%- endfor %}

    // Free tensors
    {%- for local in locals %}
    comp->g_ort->ReleaseValue({{ local.nameOut }}_tensor);
    {%- endfor %}
    {%- for output in outputs %}
    comp->g_ort->ReleaseValue({{ output.name }}_tensor);
    {%- endfor %}
    {%- for local in locals %}
    comp->g_ort->ReleaseValue({{ local.nameIn }}_tensor);
    {%- endfor %}
    {%- for input in inputs %}
    comp->g_ort->ReleaseValue({{ input.name }}_tensor);
    {%- endfor %}

    // Free arrays
    {%- for local in locals %}
    free({{ local.nameIn }}_float);
    {%- endfor %}
    {%- for input in inputs %}
    free({{ input.name }}_float);
    {%- endfor %}

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
        {%- for input in inputs %}
        {%- for scalar in input.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in outputs %}
        {%- for scalar in output.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Locals
        {%- for local in locals %}
        {%- for scalar in local.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
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
        {%- for input in inputs %}
        {%- for scalar in input.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            M({{ scalar.name }}) = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Locals
        {%- for local in locals %}
        {%- for scalar in local.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            M({{ scalar.name }}) = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
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
        {%- for input in inputs %}
        {%- for scalar in input.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in outputs %}
        {%- for scalar in output.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Locals
        {%- for local in locals %}
        {%- for scalar in local.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
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
        {%- for input in inputs %}
        {%- for scalar in input.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            M({{ scalar.name }}) = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Locals
        {%- for local in locals %}
        {%- for scalar in local.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            M({{ scalar.name }}) = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
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

