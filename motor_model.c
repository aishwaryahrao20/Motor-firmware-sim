#include "motor_model.h"
#include <math.h>

// Motor parameters
#define MAX_RPM        3000.0f
#define TICKS_PER_REV  600
#define THERMAL_TAU    10.0f
#define AMBIENT_TEMP   25.0f
#define MAX_TEMP_RISE  40.0f

static float   g_duty    = 0.0f;
static float   g_rpm     = 0.0f;
static float   g_temp    = AMBIENT_TEMP;
static int32_t g_ticks   = 0;

void motor_model_init(void) {
    g_duty  = 0.0f;
    g_rpm   = 0.0f;
    g_temp  = AMBIENT_TEMP;
    g_ticks = 0;
}

void motor_model_set_pwm(float duty_percent) {
    if (duty_percent < 0.0f)   duty_percent = 0.0f;
    if (duty_percent > 100.0f) duty_percent = 100.0f;
    g_duty = duty_percent;
}

// Call this every dt seconds from main loop
void motor_model_update(float dt) {
    // First-order RPM response
    float target_rpm = (g_duty / 100.0f) * MAX_RPM;
    float tau = 0.5f; // motor time constant seconds
    g_rpm += (target_rpm - g_rpm) * (dt / tau);

    // Accumulate encoder ticks
    float revs = (g_rpm / 60.0f) * dt;
    g_ticks += (int32_t)(revs * TICKS_PER_REV);

    // Thermal model
    float load = g_duty / 100.0f;
    float target_temp = AMBIENT_TEMP + load * load * MAX_TEMP_RISE;
    g_temp += (target_temp - g_temp) * (dt / THERMAL_TAU);
}

float motor_model_get_rpm(void) {
    return g_rpm;
}

int32_t motor_model_get_ticks(void) {
    return g_ticks;
}

float motor_model_get_temp(void) {
    return g_temp;
}