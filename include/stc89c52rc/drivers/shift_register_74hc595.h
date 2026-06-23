#ifndef STC89C52RC_DRIVERS_SHIFT_REGISTER_74HC595_H
#define STC89C52RC_DRIVERS_SHIFT_REGISTER_74HC595_H

#include <stdint.h>
#include <stc89c52rc/hal/gpio.h>

typedef struct {
    hal_gpio_pin_t data;
    hal_gpio_pin_t clock;
    hal_gpio_pin_t latch;
    uint16_t pulse_delay_us;
} drv_74hc595_t;

void drv_74hc595_init(const drv_74hc595_t *driver);
void drv_74hc595_write(const drv_74hc595_t *driver, uint8_t value);

#endif
