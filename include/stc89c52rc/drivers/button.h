#ifndef STC89C52RC_DRIVERS_BUTTON_H
#define STC89C52RC_DRIVERS_BUTTON_H

#include <stdbool.h>
#include <stc89c52rc/hal/gpio.h>

typedef enum {
    DRV_BUTTON_ACTIVE_HIGH = 0,
    DRV_BUTTON_ACTIVE_LOW = 1
} drv_button_polarity_t;

typedef struct {
    hal_gpio_pin_t pin;
    drv_button_polarity_t polarity;
} drv_button_t;

void drv_button_init(const drv_button_t *button);
bool drv_button_is_pressed(const drv_button_t *button);

#endif
