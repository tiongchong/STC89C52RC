#include <stc89c52rc/config.h>
#include <stc89c52rc/hal/delay.h>

void hal_delay_cycles(uint16_t cycles)
{
    volatile uint16_t remaining = cycles;

    while (remaining != 0u) {
        remaining--;
    }
}

void hal_delay_us(uint16_t us)
{
    while (us != 0u) {
        volatile uint8_t loops =
            (uint8_t)((F_CPU / STC89C52RC_MACHINE_CYCLE_DIVIDER / 1000000UL) + 1UL);

        while (loops != 0u) {
            loops--;
        }
        us--;
    }
}

void hal_delay_ms(uint16_t ms)
{
    while (ms != 0u) {
        hal_delay_us(1000u);
        ms--;
    }
}
