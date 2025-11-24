#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER example2
#define INSTANTIATION_TOKEN "033442eb-0c6b-4e93-89f7-62c604ac94c7"

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
    vr_x_1_0,
    vr_x_2_0,
    vr_x_3_0,
    vr_y_0,
    vr_y_1,
} ValueReference;

typedef struct {
    // Always include time
    double time;
    double x_1_0;
    double x_2_0;
    double x_3_0;
    double y_0;
    double y_1;
} ModelData;

#endif /* config_h */
