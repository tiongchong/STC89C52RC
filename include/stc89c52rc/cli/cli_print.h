#ifndef CLI_PRINT_H
#define CLI_PRINT_H

#include <stdint.h>
#include <stdarg.h>

/**
 * Initialize CLI print system (initializes UART if needed)
 */
void cli_print_init(void);

/**
 * Printf-style output to CLI (via UART)
 * Supports %s, %d, %u, %x, %X, %c, %%, and small width modifiers.
 */
void cli_printf(const char *fmt, ...);

/**
 * Print a newline
 */
void cli_putln(void);

/**
 * Print a string directly (no formatting)
 */
void cli_puts(const char *str);

/**
 * Print a single character (used for echo)
 */
void cli_putc(uint8_t c);

#endif // CLI_PRINT_H
