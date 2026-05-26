#ifndef RTOS_SIM_H
#define RTOS_SIM_H

#include <stdint.h>
#include <pthread.h>

#define MAX_TASKS 8

typedef void (*TaskFunc)(void *arg);

typedef struct {
    const char  *name;
    TaskFunc     func;
    void        *arg;
    uint32_t     period_ms;
    pthread_t    thread;
    int          active;
} Task;

void rtos_init(void);
void rtos_task_create(const char *name, TaskFunc func,
                      void *arg, uint32_t period_ms);
void rtos_start(void);  // blocks forever

// Shared mutex for data between tasks
extern pthread_mutex_t g_data_mutex;

#endif