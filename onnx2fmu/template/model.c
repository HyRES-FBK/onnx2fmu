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
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    M({{ scalar.name }}) = ({{ input.vType.CType }}) {{ scalar.start }};
    {%- endfor %}
    {%- endfor %}
    // Feedback states
    {%- for feedbackState in feedbackStates %}
    {%- for scalar in feedbackState.scalarValues %}
    M({{ scalar.name }}) = ({{ feedbackState.vType.CType }}) {{ scalar.start }};
    {%- endfor %}
    {%- endfor %}
}

Status initModel(ModelInstance *comp) {

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateCpuMemoryInfo(
            OrtArenaAllocator, OrtMemTypeDefault, &comp->modelBuffers.memory_info
        ),
        comp);

    {%- for input in inputs %}

    // Allocate persistent buffer for {{ input.name }}
    const size_t {{ input.name }}_shape[] = { {{ input.shape|join(", ") }} };
    const size_t {{ input.name }}_dim = sizeof({{ input.name }}_shape) / sizeof({{ input.name }}_shape[0]);
    size_t {{ input.name }}_size = 1;
    for (size_t i = 0; i < {{ input.name }}_dim; ++i) {
        {{ input.name }}_size *= {{ input.name }}_shape[i];
    }
    comp->modelBuffers.{{ input.name }}_float = (float*)calloc({{ input.name }}_size, sizeof(float));
    if (comp->modelBuffers.{{ input.name }}_float == NULL) {
        logError(comp, "Failed to allocate memory for {{ input.name }}_float");
        return Error;
    }
    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            comp->modelBuffers.memory_info,
            comp->modelBuffers.{{ input.name }}_float,
            {{ input.name }}_size * sizeof(float),
            (const int64_t*){{ input.name }}_shape,
            {{ input.name }}_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &comp->modelBuffers.{{ input.name }}_tensor
        ),
        comp);

    int {{ input.name }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            comp->modelBuffers.{{ input.name }}_tensor,
            &{{ input.name }}_is_tensor),
            comp
        );
    assert({{ input.name }}_is_tensor);
    {%- endfor %}

    {%- for feedbackState in feedbackStates %}

    // Allocate persistent buffer for {{ feedbackState.nameIn }}
    const size_t {{ feedbackState.nameIn }}_shape[] = { {{ feedbackState.shape|join(", ") }} };
    const size_t {{ feedbackState.nameIn }}_dim = sizeof({{ feedbackState.nameIn }}_shape) / sizeof({{ feedbackState.nameIn }}_shape[0]);
    size_t {{ feedbackState.nameIn }}_size = 1;
    for (size_t i = 0; i < {{ feedbackState.nameIn }}_dim; ++i) {
        {{ feedbackState.nameIn }}_size *= {{ feedbackState.nameIn }}_shape[i];
    }
    comp->modelBuffers.{{ feedbackState.nameIn }}_float = (float*)calloc({{ feedbackState.nameIn }}_size, sizeof(float));
    if (comp->modelBuffers.{{ feedbackState.nameIn }}_float == NULL) {
        logError(comp, "Failed to allocate memory for {{ feedbackState.nameIn }}_float");
        return Error;
    }
    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            comp->modelBuffers.memory_info,
            comp->modelBuffers.{{ feedbackState.nameIn }}_float,
            {{ feedbackState.nameIn }}_size * sizeof(float),
            (const int64_t*){{ feedbackState.nameIn }}_shape,
            {{ feedbackState.nameIn }}_dim,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT,
            &comp->modelBuffers.{{ feedbackState.nameIn }}_tensor
        ),
        comp);

    int {{ feedbackState.nameIn }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            comp->modelBuffers.{{ feedbackState.nameIn }}_tensor,
            &{{ feedbackState.nameIn }}_is_tensor),
            comp
        );
    assert({{ feedbackState.nameIn }}_is_tensor);
    {%- endfor %}

    return OK;
}

void freeModel(ModelInstance *comp) {

    {%- for input in inputs %}
    if (comp->modelBuffers.{{ input.name }}_tensor)
        comp->g_ort->ReleaseValue(comp->modelBuffers.{{ input.name }}_tensor);
    if (comp->modelBuffers.{{ input.name }}_float)
        free(comp->modelBuffers.{{ input.name }}_float);
    {%- endfor %}
    {%- for feedbackState in feedbackStates %}
    if (comp->modelBuffers.{{ feedbackState.nameIn }}_tensor)
        comp->g_ort->ReleaseValue(comp->modelBuffers.{{ feedbackState.nameIn }}_tensor);
    if (comp->modelBuffers.{{ feedbackState.nameIn }}_float)
        free(comp->modelBuffers.{{ feedbackState.nameIn }}_float);
    {%- endfor %}
    if (comp->modelBuffers.memory_info)
        comp->g_ort->ReleaseMemoryInfo(comp->modelBuffers.memory_info);
}

Status calculateValues(ModelInstance *comp) {

    // Update input buffers with current model values
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    comp->modelBuffers.{{ input.name }}_float[{{ loop.index0 }}] = (float)M({{ scalar.name }});
    {%- endfor %}
    {%- endfor %}

    // Time inputs carry no FMI scalars of their own (they read the FMI
    // independent variable), so their buffer is filled explicitly here from
    // comp->time -- the field kept current by fmi{2,3}SetTime (Model
    // Exchange) and by the fixed-step Co-Simulation loop. M(time)/vr_time is
    // legacy scaffolding that nothing in the runtime ever writes to, so it
    // must not be used as the source of truth for the independent variable.
    {%- for input in inputs %}
    {%- if input.isTime %}
    comp->modelBuffers.{{ input.name }}_float[0] = (float)comp->time;
    {%- endif %}
    {%- endfor %}

    // Update feedback state buffers with current model values
    {%- for feedbackState in feedbackStates %}
    {%- for scalar in feedbackState.scalarValues %}
    comp->modelBuffers.{{ feedbackState.nameIn }}_float[{{ loop.index0 }}] = (float)M({{ scalar.name }});
    {%- endfor %}
    {%- endfor %}

    // Output tensors are allocated by Run() on each call
    {%- for output in outputs %}
    OrtValue* {{ output.name }}_tensor = NULL;
    {%- endfor %}
    {%- for feedbackState in feedbackStates %}
    OrtValue* {{ feedbackState.nameOut }}_tensor = NULL;
    {%- endfor %}

    // Declare input node names
    const char* input_names[] = {
        {%- for input in inputs %}
        "{{ input.name }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if feedbackStates %},{% endif %}
        {%- for feedbackState in feedbackStates %}
        "{{ feedbackState.nameIn }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Declare output node names
    const char* output_names[] = {
        {%- for output in outputs %}
        "{{ output.name }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if feedbackStates %},{% endif %}
        {%- for feedbackState in feedbackStates %}
        "{{ feedbackState.nameOut }}"{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Gather persistent input tensors
    const OrtValue* input_tensors[] = {
        {%- for input in inputs %}
        comp->modelBuffers.{{ input.name }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if feedbackStates %},{% endif %}
        {%- for feedbackState in feedbackStates %}
        comp->modelBuffers.{{ feedbackState.nameIn }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Gather output tensors
    OrtValue* output_tensors[] = {
        {%- for output in outputs %}
        {{ output.name }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
        {%- if feedbackStates %},{% endif %}
        {%- for feedbackState in feedbackStates %}
        {{ feedbackState.nameOut }}_tensor{% if not loop.last %},{% endif %}
        {%- endfor %}
    };

    // Run inference
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(
            comp->session,
            NULL,
            input_names,
            input_tensors,
            {{ inputs|length + feedbackStates|length }},
            output_names,
            {{ outputs|length + feedbackStates|length }},
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

    // Check feedback state output tensors to be tensors
    {%- for feedbackState in feedbackStates %}
    int {{ feedbackState.nameOut }}_is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(
            output_tensors[{{ loop.index0 + outputs|length }}],
            &{{ feedbackState.nameOut }}_is_tensor),
            comp
        );
    assert ({{ feedbackState.nameOut }}_is_tensor);

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

    {%- for feedbackState in feedbackStates %}

    // Retrieve pointer to the {{ feedbackState.nameOut }} tensor
    float* {{ feedbackState.nameOut }}_tensor_data = NULL;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorMutableData(
            output_tensors[{{ loop.index0 + outputs|length }}],
            (void**)&{{ feedbackState.nameOut }}_tensor_data
        ),
        comp
    );

    // Retrieve {{ feedbackState.nameOut }} tensor info
    OrtTensorTypeAndShapeInfo* {{ feedbackState.nameOut }}_info;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetTensorTypeAndShape(
            output_tensors[{{ loop.index0 + outputs|length }}],
            &{{ feedbackState.nameOut }}_info
        ),
        comp
    );

    // Retrieve {{ feedbackState.name }} tensor shape
    size_t {{ feedbackState.nameOut }}_dims;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->GetDimensionsCount(
            {{ feedbackState.nameOut }}_info,
            &{{ feedbackState.nameOut }}_dims
        ),
        comp
    );

    // Set {{ feedbackState.nameOut }} tensor data to model
    {%- for scalar in feedbackState.scalarValues %}
    M({{ scalar.name }}) = {{ feedbackState.nameOut }}_tensor_data[{{ loop.index0 }}];
    {%- endfor %}
    {%- endfor %}

    // Release output tensors (allocated by Run on each call)
    {%- for feedbackState in feedbackStates %}
    comp->g_ort->ReleaseValue({{ feedbackState.nameOut }}_tensor);
    {%- endfor %}
    {%- for output in outputs %}
    comp->g_ort->ReleaseValue({{ output.name }}_tensor);
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
        // Feedback states
        {%- for feedbackState in feedbackStates %}
        {%- for scalar in feedbackState.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ scalar.name }});
            return OK;
        {%- endfor %}
        {%- endfor %}
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
        // Inputs
        {%- for input in inputs %}
        {%- for scalar in input.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            M({{ scalar.name }}) = (float)values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Feedback states
        {%- for feedbackState in feedbackStates %}
        {%- for scalar in feedbackState.scalarValues %}
        case vr_{{ scalar.name }}:
            ASSERT_NVALUES(1);
            M({{ scalar.name }}) = (float)values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
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
        // Feedback states
        {%- for feedbackState in feedbackStates %}
        {%- for scalar in feedbackState.scalarValues %}
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
        // Feedback states
        {%- for feedbackState in feedbackStates %}
        {%- for scalar in feedbackState.scalarValues %}
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

{%- if continuousStates %}

size_t getNumberOfContinuousStates(ModelInstance* comp) {
    UNUSED(comp);
    return {{ continuousStates|length }};
}

Status getContinuousStates(ModelInstance *comp, double x[], size_t nx) {
    UNUSED(nx);
    {%- for state in continuousStates %}
    x[{{ loop.index0 }}] = M({{ state.stateName }});
    {%- endfor %}
    return OK;
}

Status setContinuousStates(ModelInstance *comp, const double x[], size_t nx) {
    UNUSED(nx);
    {%- for state in continuousStates %}
    M({{ state.stateName }}) = x[{{ loop.index0 }}];
    {%- endfor %}
    // Defer inference to the next getContinuousStates/getDerivatives call
    // (both run calculateValues themselves) instead of running it here, so a
    // solver evaluating the derivatives right after does not trigger it twice.
    comp->isDirtyValues = true;
    return OK;
}

Status getDerivatives(ModelInstance *comp, double dx[], size_t nx) {
    UNUSED(nx);
    calculateValues(comp);
    {%- for state in continuousStates %}
    dx[{{ loop.index0 }}] = M({{ state.derivativeName }});
    {%- endfor %}
    return OK;
}
{%- endif %}

