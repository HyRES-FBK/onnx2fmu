#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER {{ name }}
#define INSTANTIATION_TOKEN "{{ GUID }}"

#define CO_SIMULATION

#define GET_FLOAT64
#define SET_FLOAT64

#define FIXED_SOLVER_STEP 1
#define DEFAULT_STOP_TIME 1

{% macro cleanName(name) -%}
{{ name | replace(":", "") }}
{%- endmacro %}

typedef enum {
    // Always include time
    vr_time,
#if FMI_VERSION < 3
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
#endif
} ValueReference;

typedef struct {
    // Always include time
    double time;
#if FMI_VERSION < 3
    {%- for input in inputs %}
    {%- for scalar in input.scalarValues %}
    double {{ cleanName(scalar.name) }};
    {%- endfor %}
    {%- endfor %}
    {%- for output in outputs %}
    {%- for scalar in output.scalarValues %}
    double {{ cleanName(scalar.name) }};
    {%- endfor %}
    {%- endfor %}
#endif
} ModelData;

#endif /* config_h */

