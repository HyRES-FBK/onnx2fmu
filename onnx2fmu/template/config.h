#ifndef config_h
#define config_h

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

{% macro cleanName(name) -%}
{{ name | replace(":", "") }}
{%- endmacro %}

typedef enum {
    // Always include time
    vr_time,
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    vr_{{ cleanName(scalar.name) }},
    {%- endfor %}
    {%- endfor %}
    {%- for output in outputs %}
    {%- for scalar in output.scalarValues %}
    vr_{{ cleanName(scalar.name) }},
    {%- endfor %}
    {%- endfor %}
    // The model should not reference local variables, they are used only
    // in the function calculateValues
} ValueReference;

typedef struct {
    // Always include time
    double time;
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    {{ input.vType.CType }} {{ cleanName(scalar.name) }};
    {%- endfor %}
    {%- endfor %}
    {%- for output in outputs %}
    {%- for scalar in output.scalarValues %}
    {{ output.vType.CType }} {{ cleanName(scalar.name) }};
    {%- endfor %}
    {%- endfor %}
    {%- for local in locals %}
    {%- for scalar in local.scalarValues %}
    {{ local.vType.CType }} {{ cleanName(scalar.name) }};
    {%- endfor %}
    {%- endfor %}
} ModelData;

#endif /* config_h */

