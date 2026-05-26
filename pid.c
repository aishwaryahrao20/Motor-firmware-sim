#include "pid.h"

void pid_init(PIDController *pid,
              float kp, float ki, float kd,
              float out_min, float out_max) {
    pid->kp         = kp;
    pid->ki         = ki;
    pid->kd         = kd;
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
    pid->output_min = out_min;
    pid->output_max = out_max;
}

float pid_compute(PIDController *pid,
                  float setpoint, float measured, float dt) {
    float error = setpoint - measured;

    pid->integral += error * dt;

    // Anti-windup clamp
    if (pid->integral > pid->output_max) pid->integral = pid->output_max;
    if (pid->integral < pid->output_min) pid->integral = pid->output_min;

    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error  = error;

    float output = pid->kp * error
                 + pid->ki * pid->integral
                 + pid->kd * derivative;

    // Clamp output
    if (output > pid->output_max) output = pid->output_max;
    if (output < pid->output_min) output = pid->output_min;

    return output;
}

void pid_reset(PIDController *pid) {
    pid->integral   = 0.0f;
    pid->prev_error = 0.0f;
}