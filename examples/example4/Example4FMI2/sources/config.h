#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER example4
#define INSTANTIATION_TOKEN "1753ebf7-99f8-40fb-9a3e-ac7534368c4a"

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
    vr_u_0_0,
    vr_u_0_1,
    vr_u_0_2,
    vr_u_0_3,
    vr_u_0_4,
    vr_x_0_0,
    vr_x_0_1,
    vr_x_0_2,
    vr_X_X1_0_0,
    vr_X_X1_0_1,
    vr_X_X1_0_2,
    vr_X_X1_1_0,
    vr_X_X1_1_1,
    vr_X_X1_1_2,
    vr_X_X1_2_0,
    vr_X_X1_2_1,
    vr_X_X1_2_2,
    vr_X_X1_3_0,
    vr_X_X1_3_1,
    vr_X_X1_3_2,
    vr_X_X1_4_0,
    vr_X_X1_4_1,
    vr_X_X1_4_2,
    vr_X_X1_5_0,
    vr_X_X1_5_1,
    vr_X_X1_5_2,
    vr_X_X1_6_0,
    vr_X_X1_6_1,
    vr_X_X1_6_2,
    vr_X_X1_7_0,
    vr_X_X1_7_1,
    vr_X_X1_7_2,
    vr_X_X1_8_0,
    vr_X_X1_8_1,
    vr_X_X1_8_2,
    vr_X_X1_9_0,
    vr_X_X1_9_1,
    vr_X_X1_9_2,
    vr_U_U1_0_0,
    vr_U_U1_0_1,
    vr_U_U1_0_2,
    vr_U_U1_0_3,
    vr_U_U1_0_4,
    vr_U_U1_1_0,
    vr_U_U1_1_1,
    vr_U_U1_1_2,
    vr_U_U1_1_3,
    vr_U_U1_1_4,
    vr_U_U1_2_0,
    vr_U_U1_2_1,
    vr_U_U1_2_2,
    vr_U_U1_2_3,
    vr_U_U1_2_4,
    vr_U_U1_3_0,
    vr_U_U1_3_1,
    vr_U_U1_3_2,
    vr_U_U1_3_3,
    vr_U_U1_3_4,
    vr_U_U1_4_0,
    vr_U_U1_4_1,
    vr_U_U1_4_2,
    vr_U_U1_4_3,
    vr_U_U1_4_4,
    vr_U_U1_5_0,
    vr_U_U1_5_1,
    vr_U_U1_5_2,
    vr_U_U1_5_3,
    vr_U_U1_5_4,
    vr_U_U1_6_0,
    vr_U_U1_6_1,
    vr_U_U1_6_2,
    vr_U_U1_6_3,
    vr_U_U1_6_4,
    vr_U_U1_7_0,
    vr_U_U1_7_1,
    vr_U_U1_7_2,
    vr_U_U1_7_3,
    vr_U_U1_7_4,
    vr_U_U1_8_0,
    vr_U_U1_8_1,
    vr_U_U1_8_2,
    vr_U_U1_8_3,
    vr_U_U1_8_4,
} ValueReference;

typedef struct {
    // Always include time
    double time;
    double u_0_0;
    double u_0_1;
    double u_0_2;
    double u_0_3;
    double u_0_4;
    double x_0_0;
    double x_0_1;
    double x_0_2;
    double X_X1_0_0;
    double X_X1_0_1;
    double X_X1_0_2;
    double X_X1_1_0;
    double X_X1_1_1;
    double X_X1_1_2;
    double X_X1_2_0;
    double X_X1_2_1;
    double X_X1_2_2;
    double X_X1_3_0;
    double X_X1_3_1;
    double X_X1_3_2;
    double X_X1_4_0;
    double X_X1_4_1;
    double X_X1_4_2;
    double X_X1_5_0;
    double X_X1_5_1;
    double X_X1_5_2;
    double X_X1_6_0;
    double X_X1_6_1;
    double X_X1_6_2;
    double X_X1_7_0;
    double X_X1_7_1;
    double X_X1_7_2;
    double X_X1_8_0;
    double X_X1_8_1;
    double X_X1_8_2;
    double X_X1_9_0;
    double X_X1_9_1;
    double X_X1_9_2;
    double U_U1_0_0;
    double U_U1_0_1;
    double U_U1_0_2;
    double U_U1_0_3;
    double U_U1_0_4;
    double U_U1_1_0;
    double U_U1_1_1;
    double U_U1_1_2;
    double U_U1_1_3;
    double U_U1_1_4;
    double U_U1_2_0;
    double U_U1_2_1;
    double U_U1_2_2;
    double U_U1_2_3;
    double U_U1_2_4;
    double U_U1_3_0;
    double U_U1_3_1;
    double U_U1_3_2;
    double U_U1_3_3;
    double U_U1_3_4;
    double U_U1_4_0;
    double U_U1_4_1;
    double U_U1_4_2;
    double U_U1_4_3;
    double U_U1_4_4;
    double U_U1_5_0;
    double U_U1_5_1;
    double U_U1_5_2;
    double U_U1_5_3;
    double U_U1_5_4;
    double U_U1_6_0;
    double U_U1_6_1;
    double U_U1_6_2;
    double U_U1_6_3;
    double U_U1_6_4;
    double U_U1_7_0;
    double U_U1_7_1;
    double U_U1_7_2;
    double U_U1_7_3;
    double U_U1_7_4;
    double U_U1_8_0;
    double U_U1_8_1;
    double U_U1_8_2;
    double U_U1_8_3;
    double U_U1_8_4;
} ModelData;

#endif /* config_h */
