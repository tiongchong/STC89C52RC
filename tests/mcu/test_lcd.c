#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/drivers/lcd1602.h"

/**
 * LCD display test - Display text on the LCD
 * Usage: test lcd.display [row=<0|1>] [text=<string>]
 */
int test_lcd_display(int argc, char *argv[]) __reentrant
{
    uint32_t row = test_arg_u32(argc, argv, "row", 0);
    const char *text = test_arg_value(argc, argv, "text");
    
    if (!text) {
        text = "STC89C52RC CLI";
    }
    
    if (row > 1) row = 0;
    
    lcd_clear();
    lcd_set_cursor(row, 0);
    lcd_puts(text);
    
    cli_printf("LCD: row=%u, text='%s'\r\n", (unsigned)row, text);
    
    return test_pass();
}
