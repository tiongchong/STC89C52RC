#include <stc89c52rc/compiler.h>
#include <stc89c52rc/hal/gpio.h>
#include <stc89c52rc/mcu.h>

static STC_IDATA uint8_t gpio_latch[4] = { 0xffu, 0xffu, 0xffu, 0xffu };

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
    if (port < 4u) {
        gpio_latch[port] = value;
    }

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

void hal_gpio_write(const hal_gpio_pin_t *pin, uint8_t high)
{
    uint8_t value;
    uint8_t mask;

    if (pin == 0) {
        return;
    }

    value = hal_gpio_get_port(pin->port);
    if (pin->port < 4u) {
        value = gpio_latch[pin->port];
    }
    mask = gpio_mask(pin->bit);

    if (high) {
        value = (uint8_t)(value | mask);
    } else {
        value = (uint8_t)(value & (uint8_t)(~mask));
    }

    hal_gpio_set_port(pin->port, value);
}

uint8_t hal_gpio_read(const hal_gpio_pin_t *pin)
{
    if (pin == 0) {
        return 0;
    }

    return (hal_gpio_get_port(pin->port) & gpio_mask(pin->bit)) != 0u;
}

void hal_gpio_toggle(const hal_gpio_pin_t *pin)
{
    hal_gpio_write(pin, !hal_gpio_read(pin));
}
