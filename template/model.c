#include "config.h"
#include "model.h"
#include "onnxruntime_c_api.h"

#include <stdio.h>
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

// Import Jinja2 macros
{% from 'template/config.h' import cleanName %}

// Function to flatten a multi-dimensional array
void flatten_array(double* input, float* output, int64_t* index, const int64_t* shape, int64_t dim, int64_t current_dim) {
    if (current_dim == dim - 1) {
        for (int64_t i = 0; i < shape[current_dim]; ++i) {
            output[*index] = (float)input[i];
            (*index)++;
        }
    } else {
        for (int64_t i = 0; i < shape[current_dim]; ++i) {
            flatten_array(input + i * shape[current_dim + 1], output, index, shape, dim, current_dim + 1);
        }
    }
}

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

    // Create input tensors
    {%- for input in input_entries %}
    OrtValue* {{ cleanName(input.name) }}_tensor;

    const int64_t {{ cleanName(input.name) }}_shape[] = { {{ input.dimensions|join(", ") }} };

    // Determine the dimensions of the input tensor
    const int64_t {{ cleanName(input.name) }}_dim = sizeof({{ cleanName(input.name) }}_shape) / sizeof({{ cleanName(input.name) }}_shape[0]);
    int64_t {{ cleanName(input.name) }}_size = 1;
    for (int64_t i = 0; i < {{ cleanName(input.name) }}_dim; ++i) {
        {{ cleanName(input.name) }}_size *= {{ cleanName(input.name) }}_shape[i];
    }

    // Flatten the input array
    float* input_float = (float*)malloc({{ cleanName(input.name) }}_size * sizeof(float));
    int64_t index = 0;
    flatten_array(
        (double*)M({{ cleanName(input.name) }}), input_float, &index,
        {{ cleanName(input.name) }}_shape, {{ cleanName(input.name) }}_dim, 0
    );

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            input_float, {{ cleanName(input.name) }}_size * sizeof(float),
            {{ cleanName(input.name) }}_shape, {{ cleanName(input.name) }}_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &{{ cleanName(input.name) }}_tensor
        ),
        comp);

    int is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor({{ cleanName(input.name) }}_tensor, &is_tensor),
        comp);

    assert(is_tensor);

    {%- endfor %}

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Run inference
    OrtValue* output_tensor = NULL;
    const char* input_names[] = { "{{ input_entries[0].name }}" };
    const char* output_names[] = { "{{ output_entries[0].name }}" };
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(
            comp->session, NULL, input_names,
            (const OrtValue* const*) &{{ cleanName(input_entries[0].name) }}_tensor,
            1, output_names, 1, &output_tensor
        ),
        comp
    );

    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(output_tensor, &is_tensor),
        comp);

    assert(is_tensor);

    // Retrieve a pointer to output tensor data
    float* output_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(output_tensor, (void**)&output_tensor_data),
        comp);

    OrtTensorTypeAndShapeInfo* tensor_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(output_tensor, &tensor_info),
        comp);
    // Output tensor dimension
    size_t output_tensor_dim;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorShapeElementCount(tensor_info, &output_tensor_dim),
        comp);

    {%- for output in output_entries %}
    for (size_t i = 0; i < output_tensor_dim; i++) {
        float f = output_tensor_data[i];
        M({{ output_entries[0].name }})[{{ loop.index0 }}][i] = f;
    }
    {%- endfor %}

    // Release the input tensor
    {%- for input in input_entries %}
    comp->g_ort->ReleaseValue({{ cleanName(input.name) }}_tensor);
    {%- endfor %}
    // Release the output tensor
    {%- for output in output_entries %}
    comp->g_ort->ReleaseValue({{ cleanName(output.name) }}_tensor);
    {%- endfor %}

    free(input_float);

    return OK;
}


Status getFloat64(ModelInstance *comp, ValueReference vr, double values[],
                  size_t nValues, size_t *index) {

    // Calculate values
    calculateValues(comp);

    switch (vr)
    {
        // Time is always a double value
        case vr_time:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(time);
            return OK;
#if FMI_VERSION < 3
        // Inputs
        {%- for input in input_entries %}
        {%- for i, idx in input.indexes.items() %}
        case vr_{{ cleanName(input.name) }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ cleanName(input.name) }}){% for j in idx %}[{{ j }}]{% endfor %};
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in output_entries %}
        {%- for i, idx in output.indexes.items() %}
        case vr_{{ cleanName(output.name) }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ cleanName(output.name) }}){% for j in idx %}[{{ j }}]{% endfor %};
            return OK;
        {%- endfor %}
        {%- endfor %}
#endif
        default:
            // Compose message for log with value reference
            logError(comp, "GetFloat64: ValueReference %d not available.", vr);
            return Error;
    }
}


Status setFloat64(ModelInstance *comp, ValueReference vr, const double values[],
                  size_t nValues, size_t *index) {
    // Switch for value references
    switch (vr)
    {
        // Time is always a double value
        case vr_time:
            ASSERT_NVALUES(1);
            M(time) = values[(*index)++];
            return OK;
#if FMI_VERSION < 3
        // Inputs
        {%- for input in input_entries %}
        {%- for i, idx in input.indexes.items() %}
        case vr_{{ cleanName(input.name) }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            M({{ cleanName(input.name) }}){% for j in idx %}[{{ j }}]{% endfor %} = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        // {%- for output in output_entries %}
        // {%- for i, idx in output.indexes.items() %}
        // case vr_{{ cleanName(output.name) }}_{{ idx|join('_') }}:
        //     ASSERT_NVALUES(1);
        //     M({{ cleanName(output.name) }}){% for j in idx %}[{{ j }}]{% endfor %} = values[(*index)++];
        //     return OK;
        // {%- endfor %}
        // {%- endfor %}
#endif
    default:
        logError(comp, "SetFloat64: ValueReference %d not available.", vr);
        return Error;
    }
}


Status eventUpdate(ModelInstance *comp) {

    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;

    return OK;
}

