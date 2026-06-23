#ifndef STC89C52RC_HAL_CLOCK_H
#define STC89C52RC_HAL_CLOCK_H

#include <stdint.h>

uint32_t hal_clock_frequency_hz(void);
uint32_t hal_clock_machine_cycle_hz(void);

#endif
