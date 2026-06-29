#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/hal/uart.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void cli_print_init(void)
{
    // UART is already initialized in main, but we ensure it's ready
}

/**
 * Lightweight printf implementation for SDCC compatibility
 * Supports: %s, %d, %u, %x, %X, %c, %%
 */
void cli_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    char buf[128];
    int pos = 0;
    const char *p = fmt;
    
    while (*p && pos < (sizeof(buf) - 1)) {
        if (*p == '%' && *(p + 1)) {
            p++;
            switch (*p) {
                case 's': {
                    const char *str = va_arg(args, const char *);
                    if (str) {
                        while (*str && pos < (sizeof(buf) - 1)) {
                            buf[pos++] = *str++;
                        }
                    }
                    break;
                }
                case 'd':
                case 'i': {
                    int val = va_arg(args, int);
                    // Simple integer to string conversion
                    char num[16];
                    int num_pos = 0;
                    uint8_t is_neg = 0;
                    
                    if (val < 0) {
                        is_neg = 1;
                        val = -val;
                    }
                    
                    if (val == 0) {
                        num[num_pos++] = '0';
                    } else {
                        while (val > 0 && num_pos < 15) {
                            num[num_pos++] = '0' + (val % 10);
                            val /= 10;
                        }
                    }
                    
                    if (is_neg && pos < (sizeof(buf) - 1)) {
                        buf[pos++] = '-';
                    }
                    
                    while (num_pos > 0 && pos < (sizeof(buf) - 1)) {
                        buf[pos++] = num[--num_pos];
                    }
                    break;
                }
                case 'u': {
                    uint32_t val = va_arg(args, uint32_t);
                    char num[16];
                    int num_pos = 0;
                    
                    if (val == 0) {
                        num[num_pos++] = '0';
                    } else {
                        while (val > 0 && num_pos < 15) {
                            num[num_pos++] = '0' + (val % 10);
                            val /= 10;
                        }
                    }
                    
                    while (num_pos > 0 && pos < (sizeof(buf) - 1)) {
                        buf[pos++] = num[--num_pos];
                    }
                    break;
                }
                case 'x':
                case 'X': {
                    uint32_t val = va_arg(args, uint32_t);
                    const char *hex = (*p == 'x') ? "0123456789abcdef" : "0123456789ABCDEF";
                    char num[16];
                    int num_pos = 0;
                    
                    if (val == 0) {
                        num[num_pos++] = '0';
                    } else {
                        while (val > 0 && num_pos < 15) {
                            num[num_pos++] = hex[val & 0x0F];
                            val >>= 4;
                        }
                    }
                    
                    while (num_pos > 0 && pos < (sizeof(buf) - 1)) {
                        buf[pos++] = num[--num_pos];
                    }
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    if (pos < (sizeof(buf) - 1)) {
                        buf[pos++] = c;
                    }
                    break;
                }
                case '%': {
                    if (pos < (sizeof(buf) - 1)) {
                        buf[pos++] = '%';
                    }
                    break;
                }
                default:
                    if (pos < (sizeof(buf) - 1)) {
                        buf[pos++] = *p;
                    }
                    break;
            }
            p++;
        } else {
            buf[pos++] = *p++;
        }
    }
    
    buf[pos] = '\0';
    va_end(args);
    
    // Send via UART
    hal_uart_puts(buf);
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
