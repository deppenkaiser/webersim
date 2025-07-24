#include "typedefs.h"
#include "states.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

bool state_config(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    size_t body_index = 0;

    data->objects[body_index] = (struct simulation_object)
    {
        .body = (struct celestial_body)
        {
            .a_m = 0.0L,
            .e = 0.0L,
            .T_s = 0.0L,
            .mass_kg = PHYSICS_SUN_MASS,
            .name = "Sun"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_MERCURY_A,
            .e = PHYSICS_MERCURY_ECCENTRICITY,
            .mass_kg = PHYSICS_MERCURY_MASS,
            .name = "Mercury"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (ld) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_VENUS_A,
            .e = PHYSICS_VENUS_ECCENTRICITY,
            .mass_kg = PHYSICS_VENUS_MASS,
            .name = "Venus"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_EARTH_A,
            .e = PHYSICS_EARTH_ECCENTRICITY,
            .mass_kg = PHYSICS_EARTH_MASS + PHYSICS_MOON_MASS,
            .name = "Earth"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_MARS_A,
            .e = PHYSICS_MARS_ECCENTRICITY,
            .mass_kg = PHYSICS_MARS_SYSTEM_MASS,
            .name = "Mars"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_JUPITER_A,
            .e = PHYSICS_JUPITER_ECCENTRICITY,
            .mass_kg = PHYSICS_JUPITER_SYSTEM_MASS,
            .name = "Jupiter"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_SATURN_A,
            .e = PHYSICS_SATURN_ECCENTRICITY,
            .mass_kg = PHYSICS_SATURN_SYSTEM_MASS,
            .name = "Saturn"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_URANUS_A,
            .e = PHYSICS_URANUS_ECCENTRICITY,
            .mass_kg = PHYSICS_URANUS_MASS,
            .name = "Uranus"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0L : (double) rand() / RAND_MAX * 2.0L * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_NEPTUNE_A,
            .e = PHYSICS_NEPTUNE_ECCENTRICITY,
            .mass_kg = PHYSICS_NEPTUNE_MASS,
            .name = "Neptune"
        }
    };

    data->T_step_s = 1.0L;
    for (size_t i = 0; i < N; ++i)
    {
        data->objects[i].body.index = i;
        
        if (i > 0)
        {
            data->objects[i].body.e_square = pow(data->objects[i].body.e, 2.0L);
            data->objects[i].body.h = physics_weber_h(&data->objects[i].body, PHYSICS_SUN_MASS);
            data->objects[i].body.A = physics_weber_alpha(&data->objects[i].body, PHYSICS_SUN_MASS);
            data->objects[i].body.K_1 = physics_weber_k_1st_order(&data->objects[i].body, PHYSICS_SUN_MASS);
            data->objects[i].body.K_2 = physics_weber_k_2nd_order(&data->objects[i].body, PHYSICS_SUN_MASS);
            data->objects[i].body.T_s = physics_weber_periodtime(&data->objects[i].body, PHYSICS_SUN_MASS);
            struct vector_3d perihel_m = physics_weber_position(&data->objects[i].body, PHYSICS_SUN_MASS, 0.0L);
            data->objects[i].body.perihel_m = vector_norm(&perihel_m);
        }
    }

    next_state->state_function = state_update;
    return true;
}
