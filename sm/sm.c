#include "sm/sm.h"

void* _sm_thread(void* core)
{
    sm_core_t sm_core = (sm_core_t) core;
    while (sm_core->current_state.state_function(&sm_core->current_state, sm_core->user_data));
    return NULL;
}

void sm_run(sm_state_t next_state, void* user_data)
{
    struct sm_core sm_core = {0};
    sm_core.current_state.state_function = next_state->state_function;
    sm_core.user_data = user_data;
    sm_core.thread = threading_thread_create(_sm_thread, &sm_core);
    threading_thread_join(sm_core.thread);
}
