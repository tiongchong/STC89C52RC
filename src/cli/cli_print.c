#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/hal/uart.h"
#include <stdarg.h>

void cli_print_init(void)
{
    // UART is already initialized in main, but we ensure it's ready
}

static uint8_t cli_is_digit(char value)
{
    return (value >= '0') && (value <= '9');
}

static uint8_t cli_strlen_u8(const char *text)
{
    uint8_t length = 0u;

    while ((text != 0) && (*text != '\0') && (length < 255u)) {
        length++;
        text++;
    }

    return length;
}

static void cli_put_padding(uint8_t count, char value)
{
    while (count != 0u) {
        hal_uart_putc((uint8_t)value);
        count--;
    }
}

static void cli_put_unsigned(unsigned int value, uint8_t base, uint8_t uppercase,
                             uint8_t min_width, uint8_t pad_zero)
{
    char digits[10];
    uint8_t length = 0u;
    const char *hex = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";

    if (value == 0u) {
        digits[length++] = '0';
    } else {
        while ((value != 0u) && (length < sizeof(digits))) {
            uint8_t digit = (uint8_t)(value % base);
            digits[length++] = hex[digit];
            value /= base;
        }
    }

    if (min_width > length) {
        cli_put_padding((uint8_t)(min_width - length), pad_zero ? '0' : ' ');
    }

    while (length != 0u) {
        hal_uart_putc((uint8_t)digits[--length]);
    }
}

static void cli_put_signed(int value, uint8_t min_width, uint8_t pad_zero)
{
    unsigned int magnitude;

    if (value < 0) {
        hal_uart_putc((uint8_t)'-');
        magnitude = (unsigned int)(-value);
    } else {
        magnitude = (unsigned int)value;
    }

    cli_put_unsigned(magnitude, 10u, 0u, min_width, pad_zero);
}

void cli_printf(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);

    while ((fmt != 0) && (*fmt != '\0')) {
        uint8_t left_align = 0u;
        uint8_t pad_zero = 0u;
        uint8_t width = 0u;

        if (*fmt != '%') {
            hal_uart_putc((uint8_t)*fmt);
            fmt++;
            continue;
        }

        fmt++;
        if (*fmt == '\0') {
            break;
        }

        if (*fmt == '-') {
            left_align = 1u;
            fmt++;
        }
        if (*fmt == '0') {
            pad_zero = 1u;
            fmt++;
        }
        while (cli_is_digit(*fmt)) {
            width = (uint8_t)((width * 10u) + (uint8_t)(*fmt - '0'));
            fmt++;
        }

        switch (*fmt) {
        case 's': {
            const char *text = va_arg(args, const char *);
            uint8_t length = cli_strlen_u8(text);

            if (!left_align && (width > length)) {
                cli_put_padding((uint8_t)(width - length), ' ');
            }
            cli_puts(text);
            if (left_align && (width > length)) {
                cli_put_padding((uint8_t)(width - length), ' ');
            }
            break;
        }
        case 'd':
        case 'i':
            cli_put_signed(va_arg(args, int), width, pad_zero);
            break;
        case 'u':
            cli_put_unsigned(va_arg(args, unsigned int), 10u, 0u, width, pad_zero);
            break;
        case 'x':
            cli_put_unsigned(va_arg(args, unsigned int), 16u, 0u, width, pad_zero);
            break;
        case 'X':
            cli_put_unsigned(va_arg(args, unsigned int), 16u, 1u, width, pad_zero);
            break;
        case 'c':
            hal_uart_putc((uint8_t)va_arg(args, int));
            break;
        case '%':
            hal_uart_putc((uint8_t)'%');
            break;
        default:
            hal_uart_putc((uint8_t)'%');
            hal_uart_putc((uint8_t)*fmt);
            break;
        }

        fmt++;
    }

    va_end(args);
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
