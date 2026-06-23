#ifndef STC89C52RC_HAL_DELAY_H
#define STC89C52RC_HAL_DELAY_H

#include <stdint.h>

void hal_delay_cycles(uint16_t cycles);
void hal_delay_us(uint16_t us);
void hal_delay_ms(uint16_t ms);

#endif
