#ifndef STC89C52RC_DRIVERS_SOFT_I2C_H
#define STC89C52RC_DRIVERS_SOFT_I2C_H

#include <stdbool.h>
#include <stdint.h>
#include <stc89c52rc/hal/gpio.h>

typedef struct {
    hal_gpio_pin_t scl;
    hal_gpio_pin_t sda;
    uint16_t half_period_us;
} drv_soft_i2c_t;

void drv_soft_i2c_init(const drv_soft_i2c_t *bus);
bool drv_soft_i2c_start(const drv_soft_i2c_t *bus);
void drv_soft_i2c_stop(const drv_soft_i2c_t *bus);
bool drv_soft_i2c_write_byte(const drv_soft_i2c_t *bus, uint8_t value);
uint8_t drv_soft_i2c_read_byte(const drv_soft_i2c_t *bus, bool ack);

#endif
