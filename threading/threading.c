#include "threading/threading.h"

#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <string/string.h>

void threading_thread_sleep(enum threading_time_resolution mode, uint32_t duration)
{
    struct timespec ts = {0};

    switch (mode)
    {
        case TTR_NANO:
            ts.tv_nsec = duration;
            break;
    
        case TTR_MICRO:
            ts.tv_nsec = 1000 * duration;
            break;
    
        case TTR_MILLI:
            ts.tv_nsec = 1000 * 1000 * duration;
            break;
    
        case TTR_SECOND:
            ts.tv_sec = duration;
            break;
    }

    nanosleep(&ts, NULL); 
}

pthread_t threading_thread_create(threading_thread_function function, void* user_data)
{
    pthread_t handle = THREADING_INVALID_THREADHANDLE;
    pthread_create(&handle, NULL, function, user_data);
    return handle;
}

void threading_thread_join(pthread_t handle)
{
    pthread_join(handle, NULL);
}

void threading_critical_section_initialize(threading_critical_section_t critical_section)
{
    pthread_mutexattr_t mutexattr = {0};
    pthread_mutexattr_init(&mutexattr);
    pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(critical_section, &mutexattr);
}

void threading_critical_section_destroy(threading_critical_section_t critical_section)
{
    pthread_mutex_destroy(critical_section);
}

void threading_critical_section_lock(threading_critical_section_t critical_section)
{
    pthread_mutex_lock(critical_section);
}

void threading_critical_section_unlock(threading_critical_section_t critical_section)
{
    pthread_mutex_unlock(critical_section);
}

void threading_semaphore_initialize(threading_semaphore_t semaphore, uint32_t value)
{
    sem_init(semaphore, 0, value);
}

threading_semaphore_t threading_semaphore_open(const char* name, bool exclusive, int32_t value)
{
    string_t id = {0};
    string_copy(id, sizeof(string_t), "/");
    string_cat(id, sizeof(string_t), name);
    return sem_open(id, exclusive ? O_CREAT | O_EXCL : O_CREAT, O_RDWR, value);
}

void threading_semaphore_close(threading_semaphore_t semaphore)
{
    sem_close(semaphore);
}

void threading_semaphore_unlink(const char* name)
{
    string_t id = {0};
    string_copy(id, sizeof(string_t), "/");
    string_cat(id, sizeof(string_t), name);
    sem_unlink(id);
}

void threading_semaphore_destroy(threading_semaphore_t semaphore)
{
    sem_destroy(semaphore);
}

void threading_semaphore_increment(threading_semaphore_t semaphore)
{
    sem_post(semaphore);
}

bool threading_semaphore_wait(threading_semaphore_t semaphore, enum threading_time_resolution mode, uint32_t duration)
{
    bool no_timeout = false;
    struct timespec ts = {0};

    clock_gettime(CLOCK_REALTIME, &ts);

    switch (mode)
    {
        case TTR_NANO:
            ts.tv_nsec += duration;
            break;
    
        case TTR_MICRO:
            ts.tv_nsec += 1000 * duration;
            break;
    
        case TTR_MILLI:
            ts.tv_nsec += 1000 * 1000 * duration;
            break;
    
        case TTR_SECOND:
            ts.tv_sec += duration;
            break;
    }

    int32_t ret_value = sem_timedwait(semaphore, &ts);
    return (ret_value != ETIMEDOUT) && (ret_value != -1);
}
