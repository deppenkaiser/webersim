#pragma once

#include <stdint.h>
#include <stddef.h>
#include <physics/physics.h>

enum {N = 9, MAX_BODIES = 100};

typedef struct simulation_object
{
    struct celestial_body body;
    double phi_rad;
} *simulation_object_t;

typedef struct application_data
{
    simulation_object_t objects;
    double T;
    int32_t body_index;
    char* app_filepath;
} *application_data_t;
