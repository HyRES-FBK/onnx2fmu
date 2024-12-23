#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER {{ model_name }}
#define INSTANTIATION_TOKEN "{{ model_guid }}"

#define CO_SIMULATION

#define GET_FLOAT32
#define SET_FLOAT32

#define FIXED_SOLVER_STEP 1
#define DEFAULT_STOP_TIME 1

typedef enum {
    {%- for input in input_entries %}
    vr_{{ input.name }},
    {%- endfor %}
    {%- for output in output_entries %}
    vr_{{ output.name }},
    {%- endfor %}
} ValueReference;

typedef struct {
    {%- for input in input_entries %}
    float {{ input.name }};
    {%- endfor %}
    {%- for output in output_entries %}
    float {{ output.name }};
    {%- endfor %}
} ModelData;

#endif /* config_h */
