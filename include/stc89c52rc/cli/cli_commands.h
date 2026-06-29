#ifndef CLI_COMMANDS_H
#define CLI_COMMANDS_H

/**
 * Initialize CLI system
 * Sets up UART, welcome message, and command loop
 */
void cli_init(void);

/**
 * CLI polling function - call this regularly in main loop
 * Processes any available UART input and executes commands
 */
void cli_poll(void);

#endif // CLI_COMMANDS_H
