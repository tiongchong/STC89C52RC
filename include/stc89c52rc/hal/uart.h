#ifndef STC89C52RC_HAL_UART_H
#define STC89C52RC_HAL_UART_H

#include <stdint.h>

void hal_uart_init(uint32_t baud);
void hal_uart_putc(uint8_t value);
void hal_uart_puts(const char *text);
void hal_uart_put_hex8(uint8_t value);
void hal_uart_put_uint16(uint16_t value);
uint8_t hal_uart_getc(void);
uint8_t hal_uart_data_ready(void);

#endif
