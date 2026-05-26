#ifndef MOTOR_MODEL_H
#define MOTOR_MODEL_H

#include <stdint.h>

void    motor_model_init(void);
void    motor_model_set_pwm(float duty_percent);
float   motor_model_get_rpm(void);
int32_t motor_model_get_ticks(void);
float   motor_model_get_temp(void);
void    motor_model_update(float dt);

#endif