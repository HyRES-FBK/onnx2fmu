#include "config.h"
#include "model.h"
#include "onnxruntime_c_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include "shlwapi.h"
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


    // Create input tensors
    {%- for input in input_entries %}
    OrtValue* {{ input.name }}_tensor;

    const int64_t {{ input.name }}_shape[] = { {{ input.dimensions|join(", ") }} };

    ORT_ABORT_ON_ERROR(
        comp->g_ort->CreateTensorWithDataAsOrtValue(
            memory_info, M({{ input.name }}), sizeof(M({{ input.name }})),
            {{ input.name }}_shape, 2,
            ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT, &{{ input.name }}_tensor
        ),
        comp);

    int is_tensor;
    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor({{ input.name }}_tensor, &is_tensor),
        comp);
    {%- endfor %}

    // Release the memory info
    comp->g_ort->ReleaseMemoryInfo(memory_info);

    // Run inference
    OrtValue* output_tensor = NULL;
    const char* input_names[] = { "onnx::Gemm_0" };
    const char* output_names[] = { "4" };
    ORT_ABORT_ON_ERROR(
        comp->g_ort->Run(comp->session, NULL, input_names, (const OrtValue* const*) &{{ input_entries[0].name }}_tensor, 1, output_names, 1, &output_tensor),
        comp
    );

    ORT_ABORT_ON_ERROR(
        comp->g_ort->IsTensor(output_tensor, &is_tensor),
        comp);

    // Print output tensor values
    float* output_data;
    ORT_ABORT_ON_ERROR(comp->g_ort->GetTensorMutableData(output_tensor, (void**)&output_data), comp);

    logEvent(comp, "Output tensor values: %f, %f, %f, %f.", output_data[0], output_data[1], output_data[2], output_data[3]);

    return OK;
}


Status getFloat64(ModelInstance *comp, ValueReference vr, double values[],
                  size_t nValues, size_t *index) {
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
        case vr_{{ input.name }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ input.name }}){% for j in idx %}[{{ j }}]{% endfor %};
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in output_entries %}
        {%- for i, idx in output.indexes.items() %}
        case vr_{{ output.name }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            values[(*index)++] = M({{ output.name }}){% for j in idx %}[{{ j }}]{% endfor %};
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
        case vr_{{ input.name }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            M({{ input.name }}){% for j in idx %}[{{ j }}]{% endfor %} = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
        // Outputs
        {%- for output in output_entries %}
        {%- for i, idx in output.indexes.items() %}
        case vr_{{ output.name }}_{{ idx|join('_') }}:
            ASSERT_NVALUES(1);
            M({{ output.name }}){% for j in idx %}[{{ j }}]{% endfor %} = values[(*index)++];
            return OK;
        {%- endfor %}
        {%- endfor %}
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
