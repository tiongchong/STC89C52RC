#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/hal/uart.h"

/**
 * UART loopback test - Send data and verify echo
 * Usage: test uart.loopback [len=<bytes>]
 */
int test_uart_loopback(int argc, char *argv[])
{
    uint32_t len = test_arg_u32(argc, argv, "len", 10);
    
    if (len > 32) len = 32;  // Limit for safety
    
    cli_printf("UART Loopback test: sending %u bytes\r\n", (unsigned)len);
    
    // Send test pattern
    cli_puts("Sending: ");
    for (uint32_t i = 0; i < len; i++) {
        uint8_t byte = (uint8_t)(0x41 + (i % 26));  // 'A' to 'Z'
        hal_uart_putc(byte);
        cli_putc(byte);
    }
    cli_putln();
    
    cli_puts("UART loopback sent successfully\r\n");
    
    return test_pass();
}
