#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER {{ model_name }}
#define INSTANTIATION_TOKEN "{{ model_GUID }}"

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
    {%- for input in input_entries %}
    {%- for i, idx in input.indexes.items() %}
    vr_{{ cleanName(input.name) }}_{{ idx|join('_') }},
    {%- endfor %}
    {%- endfor %}
    {%- for output in output_entries %}
    {%- for i, idx in output.indexes.items() %}
    vr_{{ cleanName(output.name) }}_{{ idx|join('_') }},
    {%- endfor %}
    {%- endfor %}
#else
    {%- for input in input_entries %}
    vr_{{ cleanName(input.name) }},
    {%- endfor %}
    {%- for output in output_entries %}
    vr_{{ cleanName(output.name) }},
    {%- endfor %}
#endif
} ValueReference;

typedef struct {
    // Always include time
    double time;
    {%- for input in input_entries %}
    double {{ cleanName(input.name) }}{% for dim in input.dimensions %}[{{ dim }}]{% endfor %};
    {%- endfor %}
    {%- for output in output_entries %}
    double {{ cleanName(output.name) }}{% for dim in output.dimensions %}[{{ dim }}]{% endfor %};
    {%- endfor %}
} ModelData;

#endif /* config_h */
