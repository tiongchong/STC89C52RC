#include <stc89c52rc/drivers/shift_register_74hc595.h>
#include <stc89c52rc/hal/delay.h>

static void pulse_pin(const hal_gpio_pin_t *pin, uint16_t delay_us)
{
    hal_gpio_write(pin, 1u);
    hal_delay_us(delay_us);
    hal_gpio_write(pin, 0u);
    hal_delay_us(delay_us);
}

void drv_74hc595_init(const drv_74hc595_t *driver)
{
    if (driver == 0) {
        return;
    }

    hal_gpio_write(&driver->data, 0u);
    hal_gpio_write(&driver->clock, 0u);
    hal_gpio_write(&driver->latch, 0u);
}

void drv_74hc595_write(const drv_74hc595_t *driver, uint8_t value)
{
    uint8_t bit;

    if (driver == 0) {
        return;
    }

    hal_gpio_write(&driver->latch, 0u);
    for (bit = 0u; bit < 8u; bit++) {
        hal_gpio_write(&driver->data, (value & 0x80u) != 0u);
        pulse_pin(&driver->clock, driver->pulse_delay_us);
        value = (uint8_t)(value << 1u);
    }
    pulse_pin(&driver->latch, driver->pulse_delay_us);
}
