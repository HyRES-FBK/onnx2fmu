#ifndef config_h
#define config_h

#include "onnxruntime_c_api.h"

// define class name and unique id
#define MODEL_IDENTIFIER {{ name }}
#define INSTANTIATION_TOKEN "{{ GUID }}"

#define CO_SIMULATION

#define GET_FLOAT64
#define SET_FLOAT64
#if FMI_VERSION > 2
#define GET_FLOAT32
#define SET_FLOAT32
#endif

#define FIXED_SOLVER_STEP 1
#define DEFAULT_STOP_TIME 1

typedef enum {
    // Always include time
    vr_time,
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    vr_{{ scalar.name }},
    {%- endfor %}
    {%- endfor %}
    {%- for output in outputs %}
    {%- for scalar in output.scalarValues %}
    vr_{{ scalar.name }},
    {%- endfor %}
    {%- endfor %}
    {%- for local in locals %}
    {%- for scalar in local.scalarValues %}
    vr_{{ scalar.name }},
    {%- endfor %}
    {%- endfor %}
} ValueReference;

typedef struct {
    // Always include time
    double time;
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    {{ input.vType.CType }} {{ scalar.name }};
    {%- endfor %}
    {%- endfor %}
    {%- for output in outputs %}
    {%- for scalar in output.scalarValues %}
    {{ output.vType.CType }} {{ scalar.name }};
    {%- endfor %}
    {%- endfor %}
    {%- for local in locals %}
    {%- for scalar in local.scalarValues %}
    {{ local.vType.CType }} {{ scalar.name }};
    {%- endfor %}
    {%- endfor %}
} ModelData;

typedef struct {
    OrtMemoryInfo* memory_info;
    {%- for input in inputs %}
    float* {{ input.name }}_float;
    OrtValue* {{ input.name }}_tensor;
    {%- endfor %}
    {%- for local in locals %}
    float* {{ local.nameIn }}_float;
    OrtValue* {{ local.nameIn }}_tensor;
    {%- endfor %}
} ModelBuffers;

#endif /* config_h */

