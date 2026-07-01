#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/hal/uart.h"

void cli_print_init(void)
{
    // UART is already initialized in main, but we ensure it's ready
}

void cli_printf(const char *fmt, ...)
{
    (void)fmt;
    cli_puts("CLI printf stub\r\n");
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
