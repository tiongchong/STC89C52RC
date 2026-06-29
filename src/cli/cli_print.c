#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/hal/uart.h"
#include <stdio.h>
#include <stdarg.h>

void cli_print_init(void)
{
    // UART is already initialized in main, but we ensure it's ready
    // This is a no-op if UART was already initialized
}

void cli_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    // Use stack buffer to avoid malloc - safer for embedded systems
    char buf[128];
    int len = vsnprintf(buf, sizeof(buf), fmt, args);
    
    va_end(args);
    
    // Send via UART
    if (len > 0) {
        hal_uart_puts(buf);
    }
}

void cli_putln(void)
{
    hal_uart_putc('\n');
}

void cli_puts(const char *str)
{
    if (str) {
        hal_uart_puts(str);
    }
}

void cli_putc(uint8_t c)
{
    hal_uart_putc(c);
}
