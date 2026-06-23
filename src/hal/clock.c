#include <stc89c52rc/config.h>
#include <stc89c52rc/hal/clock.h>

uint32_t hal_clock_frequency_hz(void)
{
    return F_CPU;
}

uint32_t hal_clock_machine_cycle_hz(void)
{
    return F_CPU / STC89C52RC_MACHINE_CYCLE_DIVIDER;
}
