#include "hal.h"
#include "motor_model.h"
#include "pid.h"
#include "rtos_sim.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Shared state
static float g_setpoint_rpm = 1500.0f;
static float g_actual_rpm   = 0.0f;
static float g_pwm_duty     = 0.0f;

static PIDController g_pid;

// Task 1 — encoder read (1ms)
void task_encoder(void *arg) {
    pthread_mutex_lock(&g_data_mutex);
    g_actual_rpm = motor_model_get_rpm();
    motor_model_update(0.001f);
    pthread_mutex_unlock(&g_data_mutex);
}

// Task 2 — PID compute (5ms)
void task_pid(void *arg) {
    pthread_mutex_lock(&g_data_mutex);
    float measured = g_actual_rpm;
    float setpoint = g_setpoint_rpm;
    pthread_mutex_unlock(&g_data_mutex);

    float output = pid_compute(&g_pid, setpoint, measured, 0.005f);

    pthread_mutex_lock(&g_data_mutex);
    g_pwm_duty = output;
    pthread_mutex_unlock(&g_data_mutex);

    hal_pwm_set_duty(output);
}

// Task 3 — CAN comms (10ms) — simulated
void task_can(void *arg) {
    // Simulate receiving a CAN frame with new setpoint
    // In hardware this would read from CAN RX register
    uint8_t can_frame[8] = {0x05, 0xDC, 0,0,0,0,0,0}; // 0x05DC = 1500 RPM
    float new_setpoint = (float)((can_frame[0] << 8) | can_frame[1]);
    pthread_mutex_lock(&g_data_mutex);
    g_setpoint_rpm = new_setpoint;
    pthread_mutex_unlock(&g_data_mutex);
}

// Task 4 — UART telemetry (50ms)
void task_telemetry(void *arg) {
    pthread_mutex_lock(&g_data_mutex);
    float rpm  = g_actual_rpm;
    float duty = g_pwm_duty;
    float sp   = g_setpoint_rpm;
    pthread_mutex_unlock(&g_data_mutex);

    uint8_t addr = 0x48, reg = 0x00, data[2];
    hal_i2c_read(addr, reg, data, 2);
    float temp = motor_model_get_temp();

    uint8_t tx[2] = {0x82, 0x00}; // SPI config register write
    uint8_t rx[2];
    hal_spi_write(tx, rx, 2);

    char msg[128];
    snprintf(msg, sizeof(msg),
        "SP=%.0f RPM=%.1f PWM=%.1f%% TEMP=%.2fC",
        sp, rpm, duty, temp);
    hal_uart_send(msg);
}

int main(void) {
    hal_uart_send("=== Motor Firmware Simulator ===");

    motor_model_init();
    pid_init(&g_pid,
             0.8f,   // Kp
             0.3f,   // Ki
             0.02f,  // Kd
             0.0f,   // min output (duty%)
             100.0f  // max output (duty%)
    );

    rtos_init();
    rtos_task_create("encoder",   task_encoder,   NULL, 1);
    rtos_task_create("pid",       task_pid,       NULL, 5);
    rtos_task_create("can",       task_can,       NULL, 10);
    rtos_task_create("telemetry", task_telemetry, NULL, 50);
    rtos_start();

    return 0;
}