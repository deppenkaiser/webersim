#pragma once

#include <sm/sm.h>

bool state_init(sm_state_t next_state, void* user_data);
bool state_config(sm_state_t next_state, void* user_data);
//bool state_barycenter(sm_state_t next_state, void* user_data);
bool state_update(sm_state_t next_state, void* user_data);
bool state_update_angel(sm_state_t next_state, void* user_data);
//bool state_output(sm_state_t next_state, void* user_data);
//bool state_finish(sm_state_t next_state, void* user_data);
