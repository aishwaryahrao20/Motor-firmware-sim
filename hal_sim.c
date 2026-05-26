#include "hal.h"
#include "motor_model.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

void hal_uart_send(const char *msg) {
    printf("[UART] %s\n", msg);
    fflush(stdout);
}

int hal_i2c_read(uint8_t addr, uint8_t reg, uint8_t *data, int len) {
    if (addr == 0x48) {
        float temp = motor_model_get_temp();
        int16_t raw = (int16_t)(temp * 16.0f);
        if (len >= 2) {
            data[0] = (raw >> 4) & 0xFF;
            data[1] = (raw & 0x0F) << 4;
        }
        printf("[I2C]  addr=0x%02X reg=0x%02X temp=%.2fC\n", addr, reg, temp);
        return 0;
    }
    return -1;
}

int hal_spi_write(uint8_t *tx, uint8_t *rx, int len) {
    printf("[SPI]  tx=0x%02X%02X ", tx[0], tx[1]);
    if (rx && len >= 2) {
        rx[0] = 0x00;
        rx[1] = 0xAB;
        printf("rx=0x%02X%02X", rx[0], rx[1]);
    }
    printf("\n");
    return 0;
}

void hal_pwm_set_duty(float duty_percent) {
    motor_model_set_pwm(duty_percent);
    printf("[PWM]  duty=%.1f%%\n", duty_percent);
}

int32_t hal_encoder_get_ticks(void) {
    return motor_model_get_ticks();
}

void hal_delay_ms(uint32_t ms) {
    usleep(ms * 1000);
}

uint32_t hal_get_tick_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
