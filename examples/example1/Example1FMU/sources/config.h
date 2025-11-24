#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER example1
#define INSTANTIATION_TOKEN "9563bf3f-4a18-43e7-a9d2-24a94036317a"

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
    vr_scalar_input_0,
    vr_vector_input_0,
    vr_vector_input_1,
    vr_vector_input_2,
    vr_vector_input_3,
    vr_vector_input_discrete_0,
    vr_vector_input_discrete_1,
    vr_vector_input_discrete_2,
    vr_vector_input_discrete_3,
    vr_vector_input_discrete_4,
    vr_output_0,
    vr_output_1,
    vr_output_2,
    vr_output_3,
    vr_output_4,
    vr_output_5,
    vr_output_6,
    vr_output_7,
    vr_output_8,
    vr_output_9,
} ValueReference;

typedef struct {
    // Always include time
    double time;
    double scalar_input_0;
    double vector_input_0;
    double vector_input_1;
    double vector_input_2;
    double vector_input_3;
    double vector_input_discrete_0;
    double vector_input_discrete_1;
    double vector_input_discrete_2;
    double vector_input_discrete_3;
    double vector_input_discrete_4;
    double output_0;
    double output_1;
    double output_2;
    double output_3;
    double output_4;
    double output_5;
    double output_6;
    double output_7;
    double output_8;
    double output_9;
} ModelData;

#endif /* config_h */
