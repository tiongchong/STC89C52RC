#include "stc89c52rc/cli/cli_commands.h"
#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_registry.h"
#include "stc89c52rc/hal/uart.h"
#include <string.h>
#include <ctype.h>

#define CMD_BUFFER_SIZE 80
#define MAX_ARGS 10

/**
 * Command buffer state
 */
static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_pos = 0;
static bool cli_ready = false;

/**
 * Parse command buffer into argc/argv
 * Returns the number of arguments (argc)
 */
static int parse_command(char *buffer, char **argv, int max_args)
{
    int argc = 0;
    char *ptr = buffer;
    bool in_arg = false;
    
    // Skip leading whitespace
    while (*ptr && isspace(*ptr)) ptr++;
    
    while (*ptr && argc < max_args) {
        if (!isspace(*ptr)) {
            if (!in_arg) {
                argv[argc++] = ptr;  // Start of new argument
                in_arg = true;
            }
            ptr++;
        } else {
            if (in_arg) {
                *ptr = '\0';  // Null-terminate argument
                in_arg = false;
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
static void process_command(const char *cmd_line)
{
    char buffer[CMD_BUFFER_SIZE];
    strncpy(buffer, cmd_line, CMD_BUFFER_SIZE - 1);
    buffer[CMD_BUFFER_SIZE - 1] = '\0';
    
    char *argv[MAX_ARGS];
    int argc = parse_command(buffer, argv, MAX_ARGS);
    
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
        cli_printf("STC89C52RC CLI\r\n");
        cli_printf("MCU: STC89C52RC\r\n");
        cli_printf("Clock: 11.0592 MHz\r\n");
        cli_printf("RAM: 256 bytes\r\n");
        cli_printf("Flash: 8 KB\r\n");
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
            size_t count;
            const stc89c52rc_test_case_t *registry = stc89c52rc_test_registry(&count);
            cli_printf("Available tests (%u):\r\n", (unsigned)count);
            for (size_t i = 0; i < count; i++) {
                cli_printf("  %-20s - %s\r\n", registry[i].name, registry[i].help);
            }
            return;
        }
        
        // Find and run the test
        const stc89c52rc_test_case_t *test = stc89c52rc_test_find(subcmd);
        if (!test) {
            cli_printf("Test not found: %s\r\n", subcmd);
            return;
        }
        
        // Run test with remaining arguments (argc-2, &argv[2])
        int result = test->run(argc - 2, &argv[2]);
        cli_printf("%s: %s (rc=%d)\r\n", 
                  result == 0 ? "PASS" : "FAIL",
                  test->name,
                  result);
        return;
    }
    
    // Unknown command
    cli_printf("Unknown command: %s\r\n", cmd);
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
    cli_ready = true;
    
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

/**
 * Helper to output a single character (used for echo)
 */
void cli_putc(uint8_t c)
{
    hal_uart_putc(c);
}
