#include "typedefs.h"

#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <assert.h>
#include <sm/sm.h>
#include <physics/physics.h>
#include <string/string.h>
#include <vector/vector.h>

bool state_init(sm_state_t next_state, void* user_data);
bool state_config(sm_state_t next_state, void* user_data);
bool state_barycenter(sm_state_t next_state, void* user_data);
bool state_momentum_check(sm_state_t next_state, void* user_data);
bool state_update(sm_state_t next_state, void* user_data);
bool state_update_angel(sm_state_t next_state, void* user_data);
bool state_output(sm_state_t next_state, void* user_data);
bool state_finish(sm_state_t next_state, void* user_data);

bool check_perihel(uint32_t body_of_interest, const application_data_t data, cd time)
{
    double r_m = vector_norm(&data->objects[body_of_interest].body.r_m);
    int32_t r_behavior = 0;
    bool action = false;

    if (r_m > data->perihel_last_r_m[body_of_interest])
    {
        r_behavior = 1;
    }
    else if (r_m < data->perihel_last_r_m[body_of_interest])
    {
        r_behavior = -1;
    }

    action = (data->last_r_behavior[body_of_interest] != r_behavior) &&
        (data->last_r_behavior[body_of_interest] != 0) && (r_behavior != 0);
    data->perihel_last_r_m[body_of_interest] = r_m;
    data->last_r_behavior[body_of_interest] = r_behavior;

    return action;
}

bool state_finish(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    free(data->objects);
    data->objects = NULL;
    return false; // false beendet die Zustandsmaschine
}

bool state_output(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    double pi = physics_pi();
    static double time = 0.0;

    time += data->T_step_s;

    if (data->body_of_interest == -1)
    {
        printf("%.5f: ", time / (3600.0 * 24.0));
        for (size_t i = 1; i < N; ++i)
        {
            printf("%.6f, ", vector_norm(&data->objects[i].body.r_m) / PHYSICS_AU);
            printf(i < N - 1 ? "%.2f, " : "%.2f\n", vector_norm(&data->objects[i].body.v_m_s) / 1000.0);
        }
    }
    else if (data->body_of_interest == 0)
    {
        printf("%.5f: ", time / (3600.0 * 24.0));
        printf("%s, r = %.7f km, v = %.2f km/s\n",
            data->objects[data->body_of_interest].body.name,
            vector_norm(&data->objects[data->body_of_interest].body.r_m) / 1000.0,
            vector_norm(&data->objects[data->body_of_interest].body.v_m_s) / 1000.0);
    }
    else
    {
        double r_m = vector_norm(&data->objects[data->body_of_interest].body.r_m);
        double v_m_s = vector_norm(&data->objects[data->body_of_interest].body.v_m_s);
        if (check_perihel(data->body_of_interest, data, time))
        {
            printf(COLOR_RED"\r%s"COLOR_RESET, data->objects[data->body_of_interest].body.name);
            printf(", %.5f d: ", time / (3600.0 * 24.0));
            printf("%.8f rad ", atan2(data->objects[data->body_of_interest].body.r_m.y, data->objects[data->body_of_interest].body.r_m.x));
            printf("%.8f km/s ", v_m_s / 1000.0);
            printf("%.8f AU\n", r_m / PHYSICS_AU);
        }
        else
        {
            printf(COLOR_RED"\r%s"COLOR_RESET, data->objects[data->body_of_interest].body.name);
            printf(", %.5f d: ", time / (3600.0 * 24.0));
            printf("%.8f rad ", atan2(data->objects[data->body_of_interest].body.r_m.y, data->objects[data->body_of_interest].body.r_m.x));
            printf("%.8f km/s ", v_m_s / 1000.0);
            printf("%.8f AU          ", r_m / PHYSICS_AU);
        }
    }
    fflush(NULL);

    if (time <= physics_seconds_per_day() * 365.0 * 100.0)
    {
        next_state->state_function = state_update_angel;
    }
    else
    {
        next_state->state_function = state_finish;
    }

    return true;
}

bool state_update(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;

    // Skip Sun...
    for (size_t j = 1; j < N; ++j)
    {
        data->objects[j].body.r_m = physics_weber_position(&data->objects[j].body, PHYSICS_SUN_MASS, data->objects[j].phi_rad);
        data->objects[j].body.w_rad_s = physics_weber_angular_speed(&data->objects[j].body, PHYSICS_SUN_MASS, data->objects[j].phi_rad);
        data->objects[j].body.v_m_s = vector_cross(&data->objects[j].body.w_rad_s, &data->objects[j].body.r_m);
    }

    // Clear Sun...
    vector_clear(&data->objects[0].body.r_m);
    vector_clear(&data->objects[0].body.v_m_s);
    vector_clear(&data->objects[0].body.w_rad_s);

    next_state->state_function = state_barycenter;
    return true;
}

bool state_update_angel(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    for (size_t i = 1; i < N; ++i)
    {
        // Für allgemeine 3D-Fälle:
        double delta_phi = physics_weber_deltaphi(&data->objects[i].body, PHYSICS_SUN_MASS, data->T_step_s);
        data->objects[i].phi_rad += delta_phi;
    }
    next_state->state_function = state_update;
    return true;
}

bool state_momentum_check(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    struct vector_3d sum_vector = {0};
    double sum_kg = 0.0;
    double norm = 0.0;

    for (size_t i = 0; i < N; ++i)
    {
        struct vector_3d mr = vector_multiply_scalar(&data->objects[i].body.r_bary_m, data->objects[i].body.mass_kg);
        sum_vector = vector_add(&sum_vector, &mr);
        sum_kg += data->objects[i].body.mass_kg;
    }
    sum_vector = vector_divide_scalar(&sum_vector, sum_kg);

    norm = vector_norm(&sum_vector);
    if (norm >= 1.0e-5)
    {
        printf("Position data is not valid!\n");
        return false;
    }

    vector_clear(&sum_vector);
    sum_kg = 0.0;
    for (size_t i = 0; i < N; ++i)
    {
        struct vector_3d mr = vector_multiply_scalar(&data->objects[i].body.v_bary_m_s, data->objects[i].body.mass_kg);
        sum_vector = vector_add(&sum_vector, &mr);
        sum_kg += data->objects[i].body.mass_kg;
    }
    sum_vector = vector_divide_scalar(&sum_vector, sum_kg);

    norm = vector_norm(&sum_vector);
    if (norm >= 1.0e-3)
    {
        printf("Velocity data is not valid!\n");
        return false;
    }

    next_state->state_function = state_output;
    return true;
}

bool state_barycenter(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    struct vector_3d sum_vector = {0};
    double sum_kg = 0.0;

    vector_clear(&sum_vector);
    for (size_t i = 1; i < N; ++i)
    {
        struct vector_3d mr = vector_multiply_scalar(&data->objects[i].body.r_m, data->objects[i].body.mass_kg);
        sum_vector = vector_add(&sum_vector, &mr);
        sum_kg += data->objects[i].body.mass_kg;
    }
    struct vector_3d r_bary = vector_divide_scalar(&sum_vector, -(sum_kg + data->objects[0].body.mass_kg));

    vector_clear(&sum_vector);
    sum_kg = 0.0;
    for (size_t i = 1; i < N; ++i)
    {
        struct vector_3d mv = vector_multiply_scalar(&data->objects[i].body.v_m_s, data->objects[i].body.mass_kg);
        sum_vector = vector_add(&sum_vector, &mv);
        sum_kg += data->objects[i].body.mass_kg;
    }
    struct vector_3d v_bary = vector_divide_scalar(&sum_vector, -(sum_kg + data->objects[0].body.mass_kg));

    for (size_t i = 0; i < N; ++i)
    {
        data->objects[i].body.r_bary_m = vector_add(&data->objects[i].body.r_m, &r_bary);
        data->objects[i].body.v_bary_m_s = vector_add(&data->objects[i].body.v_m_s, &v_bary);
    }

    next_state->state_function = state_momentum_check;
    return true;
}

bool state_config(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    size_t body_index = 0;

    data->T_step_s = 0.5;

    data->objects[body_index] = (struct simulation_object)
    {
        .body = (struct celestial_body)
        {
            .a_m = 0.0,
            .e = 0.0,
            .T_s = 0.0,
            .mass_kg = PHYSICS_SUN_MASS,
            .name = "Sun"
        }
    };

    data->objects[++body_index] = (struct simulation_object)
    {
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
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
        .phi_rad = data->body_of_interest == body_index ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_NEPTUNE_A,
            .e = PHYSICS_NEPTUNE_ECCENTRICITY,
            .mass_kg = PHYSICS_NEPTUNE_MASS,
            .name = "Neptune"
        }
    };

    for (size_t i = 0; i < N; ++i)
    {
        data->objects[i].body.index = i;
        
        if (i > 0)
        {
            data->objects[i].body.e_square = pow(data->objects[i].body.e, 2.0);
            data->objects[i].body.T_s = physics_weber_periodtime(&data->objects[i].body, PHYSICS_SUN_MASS);
        }
    }

    next_state->state_function = state_update;
    return true;
}

bool state_init(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    data->objects = malloc(MAX_BODIES * sizeof(struct simulation_object));
    assert(data->objects != NULL);
    memset(data->objects, 0, MAX_BODIES * sizeof(struct simulation_object));
    srand(time(NULL));
    next_state->state_function = state_config;
    return true;
}

int main(int argc, char* argv[])
{
    struct application_data data = {0};
    struct sm_state state = {0};
    state.state_function = state_init;
    data.body_of_interest = -1;
    if (argc > 1)
    {
        data.app_filepath = argv[0];
        data.body_of_interest = atoi(argv[1]);
    }
    sm_run(&state, &data);
    return 0;
}
