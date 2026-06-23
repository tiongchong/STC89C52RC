#include <stc89c52rc/config.h>
#include <stc89c52rc/hal/uart.h>
#include <stc89c52rc/mcu.h>

static uint8_t uart_timer1_reload(uint32_t baud)
{
    uint32_t divisor;

    if (baud == 0UL) {
        baud = UART_BAUD;
    }

    divisor = F_CPU / STC89C52RC_MACHINE_CYCLE_DIVIDER / 32UL / baud;
    if (divisor == 0UL) {
        divisor = 1UL;
    }
    if (divisor > 256UL) {
        divisor = 256UL;
    }

    return (uint8_t)(256UL - divisor);
}

void hal_uart_init(uint32_t baud)
{
    uint8_t reload = uart_timer1_reload(baud);

    PCON = (uint8_t)(PCON & 0x7fu);
    SCON = 0x50u;
    TMOD = (uint8_t)((TMOD & 0x0fu) | 0x20u);
    TH1 = reload;
    TL1 = reload;
    TI = 0;
    RI = 0;
    TR1 = 1;
}

void hal_uart_putc(uint8_t value)
{
    TI = 0;
    SBUF = value;
    while (TI == 0) {
    }
    TI = 0;
}

void hal_uart_puts(const char *text)
{
    while ((text != 0) && (*text != '\0')) {
        if (*text == '\n') {
            hal_uart_putc('\r');
        }
        hal_uart_putc((uint8_t)*text);
        text++;
    }
}

static uint8_t uart_hex_digit(uint8_t value)
{
    value = (uint8_t)(value & 0x0fu);
    if (value < 10u) {
        return (uint8_t)('0' + value);
    }

    return (uint8_t)('A' + value - 10u);
}

void hal_uart_put_hex8(uint8_t value)
{
    hal_uart_putc(uart_hex_digit((uint8_t)(value >> 4u)));
    hal_uart_putc(uart_hex_digit(value));
}

void hal_uart_put_uint16(uint16_t value)
{
    uint16_t divisor = 10000u;
    uint8_t digit;

    while ((divisor > value) && (divisor > 1u)) {
        divisor = (uint16_t)(divisor / 10u);
    }

    while (divisor != 0u) {
        digit = (uint8_t)(value / divisor);
        hal_uart_putc((uint8_t)('0' + digit));
        value = (uint16_t)(value % divisor);
        divisor = (uint16_t)(divisor / 10u);
    }
}

uint8_t hal_uart_getc(void)
{
    while (RI == 0) {
    }
    RI = 0;
    return SBUF;
}

uint8_t hal_uart_data_ready(void)
{
    return RI ? 1u : 0u;
}

int putchar(int c)
{
    if (c == '\n') {
        hal_uart_putc('\r');
    }
    hal_uart_putc((uint8_t)c);
    return c;
}
