#include <stc89c52rc/hal/gpio.h>
#include <stc89c52rc/mcu.h>

static uint8_t gpio_mask(uint8_t bit)
{
    return (uint8_t)(1u << (bit & 0x07u));
}

uint8_t hal_gpio_get_port(uint8_t port)
{
    switch (port) {
    case 0u:
        return P0;
    case 1u:
        return P1;
    case 2u:
        return P2;
    case 3u:
        return P3;
    default:
        return 0xffu;
    }
}

void hal_gpio_set_port(uint8_t port, uint8_t value)
{
    switch (port) {
    case 0u:
        P0 = value;
        break;
    case 1u:
        P1 = value;
        break;
    case 2u:
        P2 = value;
        break;
    case 3u:
        P3 = value;
        break;
    default:
        break;
    }
}

void hal_gpio_write(hal_gpio_pin_t pin, bool high)
{
    uint8_t value = hal_gpio_get_port(pin.port);
    uint8_t mask = gpio_mask(pin.bit);

    if (high) {
        value = (uint8_t)(value | mask);
    } else {
        value = (uint8_t)(value & (uint8_t)(~mask));
    }

    hal_gpio_set_port(pin.port, value);
}

bool hal_gpio_read(hal_gpio_pin_t pin)
{
    return (hal_gpio_get_port(pin.port) & gpio_mask(pin.bit)) != 0u;
}

void hal_gpio_toggle(hal_gpio_pin_t pin)
{
    hal_gpio_write(pin, !hal_gpio_read(pin));
}
