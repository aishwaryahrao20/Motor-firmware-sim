#ifndef PID_H
#define PID_H

typedef struct {
    float kp, ki, kd;
    float integral;
    float prev_error;
    float output_min;
    float output_max;
} PIDController;

void  pid_init(PIDController *pid,
               float kp, float ki, float kd,
               float out_min, float out_max);
float pid_compute(PIDController *pid,
                  float setpoint, float measured, float dt);
void  pid_reset(PIDController *pid);

#endif