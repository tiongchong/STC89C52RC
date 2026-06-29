#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/board.h"
#include "stc89c52rc/drivers/shift_register_74hc595.h"
#include <string.h>

static drv_74hc595_t shift_register = {
    BOARD_74HC595_DATA_PIN,
    BOARD_74HC595_CLOCK_PIN,
    BOARD_74HC595_LATCH_PIN,
    1u
};

/**
 * Shift register write test - Write pattern to 74HC595
 * Usage: test shiftreg.write [value=<0-255>] [pattern=<binary|count>]
 */
int test_shift_reg_write(int argc, char *argv[]) __reentrant
{
    uint32_t value = test_arg_u32(argc, argv, "value", 0xFF);
    const char *pattern = test_arg_value(argc, argv, "pattern");
    
    if (!pattern) {
        pattern = "binary";
    }
    
    if (value > 255) value = 255;

    drv_74hc595_init(&shift_register);
    
    if (strcmp(pattern, "count") == 0) {
        // Count pattern: write increasing values
        cli_puts("Shift register count pattern:\r\n");
        for (uint8_t i = 0; i < 8; i++) {
            drv_74hc595_write(&shift_register, (uint8_t)(1u << i));
            cli_printf("  0x%02X\r\n", 1u << i);
        }
    } else {
        // Simple binary write
        drv_74hc595_write(&shift_register, (uint8_t)value);
        cli_printf("Shift register write: 0x%02X (binary: ", (uint8_t)value);
        for (int i = 7; i >= 0; i--) {
            cli_putc((value & (1 << i)) ? '1' : '0');
        }
        cli_puts(")\r\n");
    }
    
    return test_pass();
}
