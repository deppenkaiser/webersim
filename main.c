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

    time += data->T;

    printf("%.5f: ", time / (3600.0 * 24.0));
    if (data->body_index == -1)
    {
        for (size_t i = 1; i < N; ++i)
        {
            printf(COLOR_YELLOW"%.2f, "COLOR_RESET, data->objects[i].phi_rad / (2.0 * pi));
            printf(i < N - 1 ? "%.2f %.2f, " : "%.2f %.2f\n", data->objects[i].body.r_m.x / PHYSICS_AU, data->objects[i].body.r_m.y / PHYSICS_AU);
        }
    }
    else if (data->body_index == 0)
    {
        printf("%s, x = %.2f y = %.2f, vx = %.2f vy = %.2f\n", data->objects[data->body_index].body.name,
            data->objects[data->body_index].body.r_m.x / 1000.0, data->objects[data->body_index].body.r_m.y / 1000.0,
            data->objects[data->body_index].body.v_m_s.x, data->objects[data->body_index].body.v_m_s.y);
    }
    else
    {
        printf("%s, # = %.7f, x = %.7f y = %.7f, vx = %.2f vy = %.2f\n",
            data->objects[data->body_index].body.name,
            data->objects[data->body_index].phi_rad / (2.0 * pi),
            data->objects[data->body_index].body.r_m.x / PHYSICS_AU, data->objects[data->body_index].body.r_m.y / PHYSICS_AU,
            data->objects[data->body_index].body.v_m_s.x, data->objects[data->body_index].body.v_m_s.y);
    }
    fflush(NULL);

    if (data->objects[data->body_index].phi_rad < 2.0 * pi)
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
        data->objects[j].body.v_m_s = physics_weber_velocity(&data->objects[j].body, PHYSICS_SUN_MASS, data->objects[j].phi_rad);
        data->objects[j].body.w_rad_s = physics_weber_angular_speed(&data->objects[j].body, PHYSICS_SUN_MASS, data->objects[j].phi_rad);
    }

    // Skip Sun...
    for (size_t j = 1; j < N; ++j)
    {
        struct vector_3d pr = {0};
        struct vector_3d pv = {0};
        struct vector_3d pw = {0};
        for (size_t i = 1; i < N; ++i)
        {
            if (i != j)
            {
                struct vector_3d temp_r = physics_weber_position_perturbed(&data->objects[j].body, &data->objects[i].body, PHYSICS_SUN_MASS);
                struct vector_3d temp_v = physics_weber_velocity_perturbed(&data->objects[j].body, &data->objects[i].body, PHYSICS_SUN_MASS);
                struct vector_3d temp_w = physics_weber_angular_speed_perturbed(&data->objects[j].body, &data->objects[i].body, PHYSICS_SUN_MASS);
                pr = vector_add(&pr, &temp_r);
                pv = vector_add(&pv, &temp_v);
                pw = vector_add(&pw, &temp_w);
            }
        }
        data->objects[j].body.r_m = vector_add(&data->objects[j].body.r_m, &pr);
        data->objects[j].body.v_m_s = vector_add(&data->objects[j].body.v_m_s, &pv);
        data->objects[j].body.w_rad_s = vector_add(&data->objects[j].body.w_rad_s, &pw);
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
        double delta_phi = vector_norm(&data->objects[i].body.w_rad_s) * data->T;
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
        struct vector_3d mr = vector_multiply_scalar(&data->objects[i].body.r_m, data->objects[i].body.mass_kg);
        sum_vector = vector_add(&sum_vector, &mr);
        sum_kg += data->objects[i].body.mass_kg;
    }
    sum_vector = vector_divide_scalar(&sum_vector, sum_kg);

    norm = vector_norm(&sum_vector);
    if (norm >= 1.0e-6)
    {
        printf("Position data is not valid!\n");
        return false;
    }

    vector_clear(&sum_vector);
    sum_kg = 0.0;
    for (size_t i = 0; i < N; ++i)
    {
        struct vector_3d mr = vector_multiply_scalar(&data->objects[i].body.v_m_s, data->objects[i].body.mass_kg);
        sum_vector = vector_add(&sum_vector, &mr);
        sum_kg += data->objects[i].body.mass_kg;
    }
    sum_vector = vector_divide_scalar(&sum_vector, sum_kg);

    norm = vector_norm(&sum_vector);
    if (norm >= 1.0e-10)
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
        data->objects[i].body.r_m = vector_add(&data->objects[i].body.r_m, &r_bary);
        data->objects[i].body.v_m_s = vector_add(&data->objects[i].body.v_m_s, &v_bary);
    }

    next_state->state_function = state_momentum_check;
    return true;
}

bool state_config(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;

    data->T = 1.0;

    data->objects[0] = (struct simulation_object)
    {
        .body = (struct celestial_body)
        {
            .a_m = 0.0,
            .e = 0.0,
            .mass_kg = PHYSICS_SUN_MASS,
            .name = "Sun"
        }
    };

    data->objects[1] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 1 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_MERCURY_A,
            .e = PHYSICS_MERCURY_ECCENTRICITY,
            .mass_kg = PHYSICS_MERCURY_MASS,
            .name = "Mercury"
        }
    };

    data->objects[2] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 2 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_VENUS_A,
            .e = PHYSICS_VENUS_ECCENTRICITY,
            .mass_kg = PHYSICS_VENUS_MASS,
            .name = "Venus",
        }
    };

    data->objects[3] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 3 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_EARTH_A,
            .e = PHYSICS_EARTH_ECCENTRICITY,
            .mass_kg = PHYSICS_EARTH_MASS + PHYSICS_MOON_MASS,
            .name = "Earth"
        }
    };

    data->objects[4] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 4 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_MARS_A,
            .e = PHYSICS_MARS_ECCENTRICITY,
            .mass_kg = PHYSICS_MARS_SYSTEM_MASS,
            .name = "Mars"
        }
    };

    data->objects[5] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 5 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_JUPITER_A,
            .e = PHYSICS_JUPITER_ECCENTRICITY,
            .mass_kg = PHYSICS_JUPITER_SYSTEM_MASS,
            .name = "Jupiter"
        }
    };

    data->objects[6] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 6 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_SATURN_A,
            .e = PHYSICS_SATURN_ECCENTRICITY,
            .mass_kg = PHYSICS_SATURN_SYSTEM_MASS,
            .name = "Saturn"
        }
    };

    data->objects[7] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 7 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_URANUS_A,
            .e = PHYSICS_URANUS_ECCENTRICITY,
            .mass_kg = PHYSICS_URANUS_MASS,
            .name = "Uranus"
        }
    };

    data->objects[8] = (struct simulation_object)
    {
        .phi_rad = data->body_index == 8 ? 0.0 : (double) rand() / RAND_MAX * 2.0 * physics_pi(),
        .body = (struct celestial_body)
        {
            .a_m = PHYSICS_AU * PHYSICS_NEPTUNE_A,
            .e = PHYSICS_NEPTUNE_ECCENTRICITY,
            .mass_kg = PHYSICS_NEPTUNE_MASS,
            .name = "Neptune"
        }
    };

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
    data.body_index = -1;
    if (argc > 1)
    {
        data.app_filepath = argv[0];
        data.body_index = atoi(argv[1]);
    }
    sm_run(&state, &data);
    return 0;
}
