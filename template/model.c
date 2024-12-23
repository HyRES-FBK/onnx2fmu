#include "config.h"
#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#endif

#define MAX_PATH_LENGTH 4096

void setStartValues(ModelInstance *comp) {
    UNUSED(comp);
}

Status calculateValues(ModelInstance *comp) {

    UNUSED(comp);

    return OK;
}


Status eventUpdate(ModelInstance *comp) {

    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;

    return OK;
}
