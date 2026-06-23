#ifndef STC89C52RC_HAL_TIMER_H
#define STC89C52RC_HAL_TIMER_H

#include <stdint.h>

void hal_timer0_configure_16bit(uint16_t reload);
void hal_timer0_reload(uint16_t value);
void hal_timer0_start(void);
void hal_timer0_stop(void);
uint16_t hal_timer0_read(void);
uint8_t hal_timer0_overflowed(void);
void hal_timer0_clear_overflow(void);

#endif
