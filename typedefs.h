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
    double T_step_s;
    int32_t body_of_interest;
    char* app_filepath;
    double perihel_last_r_m[MAX_BODIES];
    int32_t last_r_behavior[MAX_BODIES];
} *application_data_t;
