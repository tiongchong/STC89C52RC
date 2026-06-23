#include <stc89c52rc/hal/timer.h>
#include <stc89c52rc/mcu.h>

void hal_timer0_reload(uint16_t value)
{
    TH0 = (uint8_t)(value >> 8u);
    TL0 = (uint8_t)(value & 0xffu);
}

void hal_timer0_configure_16bit(uint16_t reload)
{
    TR0 = 0;
    TMOD = (uint8_t)((TMOD & 0xf0u) | 0x01u);
    hal_timer0_reload(reload);
    TF0 = 0;
}

void hal_timer0_start(void)
{
    TF0 = 0;
    TR0 = 1;
}

void hal_timer0_stop(void)
{
    TR0 = 0;
}

uint16_t hal_timer0_read(void)
{
    uint8_t high_first;
    uint8_t high_second;
    uint8_t low;

    do {
        high_first = TH0;
        low = TL0;
        high_second = TH0;
    } while (high_first != high_second);

    return (uint16_t)(((uint16_t)high_second << 8u) | low);
}

uint8_t hal_timer0_overflowed(void)
{
    return TF0 ? 1u : 0u;
}

void hal_timer0_clear_overflow(void)
{
    TF0 = 0;
}
