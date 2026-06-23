#include <stc89c52rc/drivers/led.h>

static bool led_output_level(const drv_led_t *led, bool on)
{
    return (led->polarity == DRV_LED_ACTIVE_LOW) ? !on : on;
}

void drv_led_init(const drv_led_t *led, bool initial_on)
{
    drv_led_set(led, initial_on);
}

void drv_led_set(const drv_led_t *led, bool on)
{
    if (led == 0) {
        return;
    }

    hal_gpio_write(led->pin, led_output_level(led, on));
}

void drv_led_on(const drv_led_t *led)
{
    drv_led_set(led, true);
}

void drv_led_off(const drv_led_t *led)
{
    drv_led_set(led, false);
}

void drv_led_toggle(const drv_led_t *led)
{
    if (led == 0) {
        return;
    }

    hal_gpio_toggle(led->pin);
}
