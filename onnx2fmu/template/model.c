{% macro cleanName(name) -%}
{{ name | replace(":", "") }}
{%- endmacro %}

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
    // Create {{ cleanName(input.name) }} tensor
    OrtValue* {{ cleanName(input.name) }}_tensor;

    // Store the shape of the input tensor
    const size_t {{ cleanName(input.name) }}_shape[] = { {{ input.shape|join(", ") }} };

    // Determine the dimensions of the input tensor
    const size_t {{ cleanName(input.name) }}_dim = sizeof({{ cleanName(input.name) }}_shape) / sizeof({{ cleanName(input.name) }}_shape[0]);
    size_t {{ cleanName(input.name) }}_size = 1;
    for (size_t i = 0; i < {{ cleanName(input.name) }}_dim; ++i) {
        {{ cleanName(input.name) }}_size *= {{ cleanName(input.name) }}_shape[i];
    }

    // Store values in the flattened array
    float* {{ cleanName(input.name) }}_float = (float*)malloc({{ cleanName(input.name) }}_size * sizeof(float));
    if ({{ cleanName(input.name) }}_float == NULL) {
        logError(comp, "Failed to allocate memory for {{ cleanName(input.name) }}_float");
        return Error;
    }

    // Flatten the input array
    {%- for scalar in input.scalarValues %}
    {{ cleanName(input.name) }}_float[{{ loop.index0 }}] = (float)M({{ cleanName(scalar.name) }});
    {%- endfor %}

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info,
            {{ cleanName(input.name) }}_float,
            {{ cleanName(input.name) }}_size * sizeof(float),
            (const int64_t*){{ cleanName(input.name) }}_shape,
            {{ cleanName(input.name) }}_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &{{ cleanName(input.name) }}_tensor
        ),
        comp);

    int {{ cleanName(input.name) }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            {{ cleanName(input.name) }}_tensor,
            &{{ cleanName(input.name) }}_is_tensor),
            comp
        );

    assert({{ cleanName(input.name) }}_is_tensor);
    {%- endfor %}

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Create output tensors
    {%- for output in outputs %}
    OrtValue* {{ cleanName(output.name) }}_tensor = NULL;
    {%- endfor %}

    // Declare input node names
    const char* input_names[] = {
        {%- for input in inputs %}
        "{{ cleanName(input.name) }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Declare output node names
    const char* output_names[] = {
        {%- for output in outputs %}
        "{{ cleanName(output.name) }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Gather input tensors
    const OrtValue* input_tensors[] = {
        {%- for input in inputs %}
        {{ cleanName(input.name) }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        {%- for output in outputs %}
        {{ cleanName(output.name) }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Run inference
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(
            comp->session,
            NULL,
            input_names,
            input_tensors,
            {{ inputs|length }},
            output_names,
            {{ outputs|length }},
            output_tensors
        ),
        comp
    );

    // Check output tensors to be tensors
    {%- for output in outputs %}
    int {{ cleanName(output.name) }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[{{ loop.index0 }}],
            &{{ cleanName(output.name) }}_is_tensor),
            comp
        );
    assert ({{ cleanName(output.name) }}_is_tensor);
    {%- endfor %}

    {%- for output in outputs %}
    // Retrieve pointer to the {{ cleanName(output.name )}} tensor
    float* {{ cleanName(output.name) }}_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[{{ loop.index0 }}],
            (void**)&{{ cleanName(output.name) }}_tensor_data
        ),
        comp
    );

    // Retrieve {{ cleanName(output.name )}} tensor info
    OrtTensorTypeAndShapeInfo* {{ cleanName(output.name) }}_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[{{ loop.index0 }}],
            &{{ cleanName(output.name) }}_info
        ),
        comp
    );

    // Retrieve {{ cleanName(output.name )}} tensor shape
    size_t {{ cleanName(output.name) }}_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            {{ cleanName(output.name) }}_info,
            &{{ cleanName(output.name) }}_dims
        ),
        comp
    );

    // Set {{ cleanName(output.name )}} tensor data to model
    {%- for scalar in output.scalarValues %}
    M({{ cleanName(scalar.name) }}) = {{ cleanName(output.name) }}_tensor_data[{{ loop.index0 }}];
    {%- endfor %}

    {%- endfor %}

    // Free tensors
    {%- for output in outputs %}
    comp->g_ort->ReleaseValue({{ cleanName(output.name) }}_tensor);
    {%- endfor %}
    {%- for input in inputs %}
    comp->g_ort->ReleaseValue({{ cleanName(input.name) }}_tensor);
    {%- endfor %}

    // Free arrays
    {%- for input in inputs %}
    free({{ cleanName(input.name) }}_float);
    {%- endfor %}

    return OK;
}

Status getFloat64(ModelInstance *comp, ValueReference vr, double values[],
                  size_t nValues, size_t *index) {

    // Calculate values
    calculateValues(comp);

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
        case vr_{{ cleanName(scalar.name) }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ cleanName(scalar.name) }});
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in outputs %}
        {%- for scalar in output.scalarValues %}
        case vr_{{ cleanName(scalar.name) }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ cleanName(scalar.name) }});
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
        case vr_{{ cleanName(scalar.name) }}:
            ASSERT_NVALUES(1);
            M({{ cleanName(scalar.name) }}) = values[(*index)++];
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

    // Calculate values
    calculateValues(comp);

    switch (vr)
    {
        // Inputs
        {%- for input in inputs %}
        {%- for scalar in input.scalarValues %}
        case vr_{{ cleanName(scalar.name) }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ cleanName(scalar.name) }});
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in outputs %}
        {%- for scalar in output.scalarValues %}
        case vr_{{ cleanName(scalar.name) }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ cleanName(scalar.name) }});
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
        case vr_{{ cleanName(scalar.name) }}:
            ASSERT_NVALUES(1);
            M({{ cleanName(scalar.name) }}) = values[(*index)++];
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

