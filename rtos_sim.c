#include "rtos_sim.h"
#include "hal.h"
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t g_data_mutex = PTHREAD_MUTEX_INITIALIZER;

static Task g_tasks[MAX_TASKS];
static int  g_task_count = 0;

void rtos_init(void) {
    g_task_count = 0;
}

// Each task runs in its own thread, looping at its period
static void *task_runner(void *arg) {
    Task *t = (Task *)arg;
    char msg[64];
    snprintf(msg, sizeof(msg), "Task started: %s @ %ums", t->name, t->period_ms);
    hal_uart_send(msg);
    while (t->active) {
        t->func(t->arg);
        hal_delay_ms(t->period_ms);
    }
    return NULL;
}

void rtos_task_create(const char *name, TaskFunc func,
                      void *arg, uint32_t period_ms) {
    if (g_task_count >= MAX_TASKS) return;
    Task *t      = &g_tasks[g_task_count++];
    t->name      = name;
    t->func      = func;
    t->arg       = arg;
    t->period_ms = period_ms;
    t->active    = 1;
}

void rtos_start(void) {
    for (int i = 0; i < g_task_count; i++) {
        pthread_create(&g_tasks[i].thread, NULL,
                       task_runner, &g_tasks[i]);
    }
    // Block main thread
    for (int i = 0; i < g_task_count; i++) {
        pthread_join(g_tasks[i].thread, NULL);
    }
}