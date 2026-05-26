#ifndef HAL_H
#define HAL_H

#include <stdint.h>

// UART
void hal_uart_send(const char *msg);

// I2C
int  hal_i2c_read(uint8_t addr, uint8_t reg, uint8_t *data, int len);

// SPI
int  hal_spi_write(uint8_t *tx, uint8_t *rx, int len);

// PWM
void hal_pwm_set_duty(float duty_percent);

// Encoder
int32_t hal_encoder_get_ticks(void);

// Timing
void    hal_delay_ms(uint32_t ms);
uint32_t hal_get_tick_ms(void);

#endif