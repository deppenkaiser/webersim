#pragma once

#include <threading/threading.h>

typedef struct sm_state *sm_state_t;
typedef bool (*sm_state_handler_t)(sm_state_t next_state, void* user_data);

struct sm_state
{
    sm_state_handler_t state_function;
};

void sm_run(sm_state_t next_state, void* user_data);

typedef struct sm_core
{
    struct sm_state current_state;
    pthread_t thread;
    void* user_data;
} *sm_core_t;
