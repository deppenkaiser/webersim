#pragma once

#include <stdint.h>
#include <stddef.h>
#include <physics/physics.h>

enum {N = 9, MAX_BODIES = 100};

typedef struct perihel_data
{
    ld phi_rad;
    ld time_d;
} perihel_data_t;

typedef struct simulation_object
{
    struct celestial_body body;
    ld phi_rad;
} *simulation_object_t;

typedef struct application_data
{
    simulation_object_t objects;
    ld T_step_s;
    ld time_s;
    int32_t body_of_interest;
    char* app_filepath;
    struct perihel_data perihel_stack[1000];
    uint32_t stack_counter;
} *application_data_t;
