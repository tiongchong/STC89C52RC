#include <stc89c52rc/drivers/soft_i2c.h>
#include <stc89c52rc/hal/delay.h>

static void i2c_delay(const drv_soft_i2c_t *bus)
{
    hal_delay_us(bus->half_period_us);
}

static void set_scl(const drv_soft_i2c_t *bus, bool high)
{
    hal_gpio_write(&bus->scl, high);
    i2c_delay(bus);
}

static void set_sda(const drv_soft_i2c_t *bus, bool high)
{
    hal_gpio_write(&bus->sda, high);
    i2c_delay(bus);
}

void drv_soft_i2c_init(const drv_soft_i2c_t *bus)
{
    if (bus == 0) {
        return;
    }

    hal_gpio_write(&bus->sda, true);
    hal_gpio_write(&bus->scl, true);
}

bool drv_soft_i2c_start(const drv_soft_i2c_t *bus)
{
    if (bus == 0) {
        return false;
    }

    set_sda(bus, true);
    set_scl(bus, true);
    if (!hal_gpio_read(&bus->sda)) {
        return false;
    }
    set_sda(bus, false);
    set_scl(bus, false);
    return true;
}

void drv_soft_i2c_stop(const drv_soft_i2c_t *bus)
{
    if (bus == 0) {
        return;
    }

    set_sda(bus, false);
    set_scl(bus, true);
    set_sda(bus, true);
}

bool drv_soft_i2c_write_byte(const drv_soft_i2c_t *bus, uint8_t value)
{
    uint8_t bit;
    bool ack;

    if (bus == 0) {
        return false;
    }

    for (bit = 0u; bit < 8u; bit++) {
        set_sda(bus, (value & 0x80u) != 0u);
        set_scl(bus, true);
        set_scl(bus, false);
        value = (uint8_t)(value << 1u);
    }

    set_sda(bus, true);
    set_scl(bus, true);
    ack = !hal_gpio_read(&bus->sda);
    set_scl(bus, false);
    return ack;
}

uint8_t drv_soft_i2c_read_byte(const drv_soft_i2c_t *bus, bool ack)
{
    uint8_t bit;
    uint8_t value = 0u;

    if (bus == 0) {
        return 0xffu;
    }

    set_sda(bus, true);
    for (bit = 0u; bit < 8u; bit++) {
        value = (uint8_t)(value << 1u);
        set_scl(bus, true);
        if (hal_gpio_read(&bus->sda)) {
            value |= 0x01u;
        }
        set_scl(bus, false);
    }

    set_sda(bus, !ack);
    set_scl(bus, true);
    set_scl(bus, false);
    set_sda(bus, true);
    return value;
}
