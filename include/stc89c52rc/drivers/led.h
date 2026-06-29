#ifndef STC89C52RC_DRIVERS_LED_H
#define STC89C52RC_DRIVERS_LED_H

#include <stdint.h>
#include <stc89c52rc/hal/gpio.h>

typedef enum {
    DRV_LED_ACTIVE_HIGH = 0,
    DRV_LED_ACTIVE_LOW = 1
} drv_led_polarity_t;

typedef struct {
    hal_gpio_pin_t pin;
    drv_led_polarity_t polarity;
} drv_led_t;

void drv_led_init(const drv_led_t *led, uint8_t initial_on);
void drv_led_set(const drv_led_t *led, uint8_t on);
void drv_led_on(const drv_led_t *led);
void drv_led_off(const drv_led_t *led);
void drv_led_toggle(const drv_led_t *led);

#endif
