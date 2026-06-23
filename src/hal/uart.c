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

void hal_uart_put_hex8(uint8_t value)
{
    static const char hex[] = "0123456789ABCDEF";

    hal_uart_putc((uint8_t)hex[(value >> 4u) & 0x0fu]);
    hal_uart_putc((uint8_t)hex[value & 0x0fu]);
}

void hal_uart_put_uint16(uint16_t value)
{
    char text[6];
    uint8_t index = 0u;

    if (value == 0u) {
        hal_uart_putc('0');
        return;
    }

    while ((value != 0u) && (index < sizeof(text))) {
        text[index++] = (char)('0' + (value % 10u));
        value = (uint16_t)(value / 10u);
    }

    while (index != 0u) {
        hal_uart_putc((uint8_t)text[--index]);
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
