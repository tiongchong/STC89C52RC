#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/drivers/shift_register_74hc595.h"
#include <string.h>

/**
 * Shift register write test - Write pattern to 74HC595
 * Usage: test shiftreg.write [value=<0-255>] [pattern=<binary|count>]
 */
int test_shift_reg_write(int argc, char *argv[])
{
    uint32_t value = test_arg_u32(argc, argv, "value", 0xFF);
    const char *pattern = test_arg_value(argc, argv, "pattern");
    
    if (!pattern) {
        pattern = "binary";
    }
    
    if (value > 255) value = 255;
    
    if (strcmp(pattern, "count") == 0) {
        // Count pattern: write increasing values
        cli_puts("Shift register count pattern:\r\n");
        for (uint8_t i = 0; i < 8; i++) {
            sr_write(1 << i);
            cli_printf("  0x%02X\r\n", 1u << i);
        }
    } else {
        // Simple binary write
        sr_write((uint8_t)value);
        cli_printf("Shift register write: 0x%02X (binary: ", (uint8_t)value);
        for (int i = 7; i >= 0; i--) {
            cli_putc((value & (1 << i)) ? '1' : '0');
        }
        cli_puts(")\r\n");
    }
    
    return test_pass();
}
