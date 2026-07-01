#include "stc89c52rc/cli/cli_commands.h"
#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_registry.h"
#include "stc89c52rc/compiler.h"
#include "stc89c52rc/hal/uart.h"
#include <string.h>

#define CMD_BUFFER_SIZE 24
#define MAX_ARGS 4

/**
 * Command buffer state
 */
static STC_XDATA char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_pos = 0;
static uint8_t cli_ready = 0;

static uint8_t cli_is_space(char value)
{
    return (value == ' ') || (value == '\t') || (value == '\r') || (value == '\n');
}

static void cli_put_u32(uint32_t value)
{
    char digits[10];
    uint8_t length = 0u;

    if (value == 0u) {
        digits[length++] = '0';
    } else {
        while ((value != 0u) && (length < sizeof(digits))) {
            digits[length++] = (char)('0' + (value % 10u));
            value /= 10u;
        }
    }

    while (length != 0u) {
        cli_putc((uint8_t)digits[--length]);
    }
}

/**
 * Parse command buffer into argc/argv
 * Returns the number of arguments (argc)
 */
static int parse_command(char STC_XDATA *buffer, char **argv, int max_args)
{
    int argc = 0;
    char STC_XDATA *ptr = buffer;
    uint8_t in_arg = 0;
    
    // Skip leading whitespace
    while (*ptr && cli_is_space(*ptr)) ptr++;
    
    while (*ptr && argc < max_args) {
        if (!cli_is_space(*ptr)) {
            if (!in_arg) {
                argv[argc++] = ptr;  // Start of new argument
                in_arg = 1;
            }
            ptr++;
        } else {
            if (in_arg) {
                *ptr = '\0';  // Null-terminate argument
                in_arg = 0;
            }
            ptr++;
        }
    }
    
    if (in_arg) {
        *ptr = '\0';  // Null-terminate last argument
    }
    
    return argc;
}

/**
 * Process a complete command
 */
static void process_command(char STC_XDATA *cmd_line)
{
    char *argv[MAX_ARGS];
    int argc = parse_command(cmd_line, argv, MAX_ARGS);
    
    if (argc == 0) {
        return;  // Empty command
    }
    
    const char *cmd = argv[0];
    
    // Handle help command
    if (strcmp(cmd, "help") == 0) {
        cli_puts("Available commands:\r\n");
        cli_puts("  help              - Show this help\r\n");
        cli_puts("  test list         - List all tests\r\n");
        cli_puts("  test <name> [args]- Run test with optional arguments\r\n");
        cli_puts("  info              - Show system info\r\n");
        return;
    }
    
    // Handle info command
    if (strcmp(cmd, "info") == 0) {
        cli_puts("STC89C52RC CLI\r\n");
        cli_puts("MCU: STC89C52RC\r\n");
        cli_puts("Clock: 11.0592 MHz\r\n");
        cli_puts("RAM: 256 bytes\r\n");
        cli_puts("Flash: 8 KB\r\n");
        return;
    }
    
    // Handle test command
    if (strcmp(cmd, "test") == 0) {
        if (argc < 2) {
            cli_puts("Usage: test <command> [args]\r\n");
            cli_puts("       test list        - List available tests\r\n");
            return;
        }
        
        const char *subcmd = argv[1];
        
        if (strcmp(subcmd, "list") == 0) {
            uint32_t count;
            const stc89c52rc_test_case_t *registry = stc89c52rc_test_registry(&count);
            cli_puts("Available tests (");
            cli_put_u32(count);
            cli_puts("):\r\n");
            for (uint32_t i = 0; i < count; i++) {
                cli_puts("  ");
                cli_puts(registry[i].name);
                cli_puts(" - ");
                cli_puts(registry[i].help);
                cli_puts("\r\n");
            }
            return;
        }
        
        // Find and run the test
        const stc89c52rc_test_case_t *test = stc89c52rc_test_find(subcmd);
        if (!test) {
            cli_puts("Test not found: ");
            cli_puts(subcmd);
            cli_puts("\r\n");
            return;
        }
        
        // Run test with remaining arguments (argc-2, &argv[2])
        int result = test->run(argc - 2, &argv[2]);
        cli_puts(result == 0 ? "PASS" : "FAIL");
        cli_puts(": ");
        cli_puts(test->name);
        cli_puts(" (rc=");
        cli_put_u32((uint32_t)result);
        cli_puts(")\r\n");
        return;
    }
    
    // Unknown command
    cli_puts("Unknown command: ");
    cli_puts(cmd);
    cli_puts("\r\n");
    cli_puts("Type 'help' for available commands\r\n");
}

/**
 * Print CLI prompt
 */
static void print_prompt(void)
{
    cli_puts("STC89C52RC> ");
}

/**
 * Initialize CLI system
 */
void cli_init(void)
{
    cli_print_init();  // Ensure printf is initialized
    cmd_pos = 0;
    cli_ready = 1;
    
    cli_puts("\r\n");
    cli_puts("========================================\r\n");
    cli_puts("   STC89C52RC Hardware Test CLI\r\n");
    cli_puts("========================================\r\n");
    cli_puts("Type 'help' for commands\r\n");
    print_prompt();
}

/**
 * CLI polling function - process UART input
 */
void cli_poll(void)
{
    if (!cli_ready) return;
    
    // Read available characters
    while (hal_uart_data_ready()) {
        uint8_t c = hal_uart_getc();
        
        // Handle backspace
        if (c == '\b' || c == 0x7F) {
            if (cmd_pos > 0) {
                cmd_pos--;
                cli_puts("\b \b");  // Backspace, space, backspace
            }
            continue;
        }
        
        // Handle newline/carriage return
        if (c == '\r' || c == '\n') {
            cli_puts("\r\n");
            
            // Process the command
            cmd_buffer[cmd_pos] = '\0';
            if (cmd_pos > 0) {
                process_command(cmd_buffer);
            }
            
            // Reset buffer and print prompt
            cmd_pos = 0;
            print_prompt();
            continue;
        }
        
        // Handle regular characters
        if (c >= 32 && c < 127 && cmd_pos < (CMD_BUFFER_SIZE - 1)) {
            cmd_buffer[cmd_pos++] = c;
            cli_putc(c);  // Echo the character
        }
    }
}
