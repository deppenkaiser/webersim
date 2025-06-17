#pragma once

#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <semaphore.h>

#define THREADING_INVALID_THREADHANDLE (pthread_t) NULL

typedef pthread_mutex_t threading_critical_section;
typedef threading_critical_section* threading_critical_section_t;
typedef sem_t threading_semaphore;
typedef threading_semaphore* threading_semaphore_t;
typedef void* (*threading_thread_function)(void* user_data);

enum threading_time_resolution
{
    TTR_NANO,
    TTR_MICRO,
    TTR_MILLI,
    TTR_SECOND
};

pthread_t threading_thread_create(threading_thread_function function, void* user_data);
void threading_thread_join(pthread_t handle);
void threading_thread_sleep(enum threading_time_resolution mode, uint32_t duration);
void threading_critical_section_initialize(threading_critical_section_t critical_section);
void threading_critical_section_destroy(threading_critical_section_t critical_section);
void threading_critical_section_lock(threading_critical_section_t critical_section);
void threading_critical_section_unlock(threading_critical_section_t critical_section);
void threading_semaphore_initialize(threading_semaphore_t semaphore, uint32_t value);
threading_semaphore_t threading_semaphore_open(const char* name, bool exclusive, int32_t value);
void threading_semaphore_close(threading_semaphore_t semaphore);
void threading_semaphore_unlink(const char* name);
void threading_semaphore_destroy(threading_semaphore_t semaphore);
void threading_semaphore_increment(threading_semaphore_t semaphore);
bool threading_semaphore_wait(threading_semaphore_t semaphore, enum threading_time_resolution mode, uint32_t duration);
