#include "typedefs.h"
#include "states.h"

#include <stdlib.h>
#include <assert.h>
#include <memory.h>

bool state_finish(sm_state_t next_state, void* user_data)
{
    application_data_t data = (application_data_t)user_data;
    free(data->objects);
    data->objects = NULL;
    return false; // false beendet die Zustandsmaschine
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
