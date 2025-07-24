#include "typedefs.h"
#include "states.h"

#include <stdio.h>
#include <math.h>
#include <sm/sm.h>
#include <physics/physics.h>
#include <string/string.h>
#include <vector/vector.h>

bool state_update(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    cld pi_2 = 2.0L * physics_pi();
    cld time_d = data->time_s / (3600.0L * 24.0L);

    for (size_t i = 1; i < N; ++i)
    {
        data->objects[i].body.r_m = physics_weber_position(&data->objects[i].body, PHYSICS_SUN_MASS, data->objects[i].phi_rad);
        data->objects[i].body.w_rad_s = physics_weber_angular_speed(&data->objects[i].body, PHYSICS_SUN_MASS, data->objects[i].phi_rad);
        data->objects[i].body.v_m_s = vector_cross(&data->objects[i].body.w_rad_s, &data->objects[i].body.r_m);

        cld r_m = vector_norm(&data->objects[i].body.r_m);
        cld v_m_s = vector_norm(&data->objects[i].body.v_m_s);

        string_set_cursor_position(1, i);
        printf(COLOR_RED"\r%s: "COLOR_RESET, data->objects[i].body.name);
        string_set_cursor_position(10, i);
        printf("%.8Lf° ", physics_rad_to_deg(data->objects[i].phi_rad));
        string_set_cursor_position(25, i);
        printf("%.2Lf km/s ", v_m_s / 1000.0L);
        string_set_cursor_position(37, i);
        printf("%.2Lf AU            ", r_m / PHYSICS_AU);
        data->objects[i].phi_rad += vector_norm(&data->objects[i].body.w_rad_s) * data->T_step_s;
        if (data->objects[i].phi_rad > pi_2)
        {
            data->objects[i].phi_rad -= pi_2;
        }
    }
    string_set_cursor_position(1, 10);
    printf("time: %.8Lf d                ", time_d);
    string_set_cursor_position(1, 11);
    fflush(NULL);

    data->time_s += data->T_step_s;
    next_state->state_function = state_update;
    return true;
}

int main(int argc, char* argv[])
{
    struct application_data data = {0};
    struct sm_state state = {0};
    state.state_function = state_init;
    data.body_of_interest = 1;
    if (argc > 1)
    {
        data.app_filepath = argv[0];
        data.body_of_interest = atoi(argv[1]);
    }
    string_clear_screen();
    sm_run(&state, &data);
    return 0;
}
