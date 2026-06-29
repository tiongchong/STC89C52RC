#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/board.h"
#include "stc89c52rc/drivers/lcd1602.h"

/**
 * LCD display test - Display text on the LCD
 * Usage: test lcd.display [row=<0|1>] [text=<string>]
 */
int test_lcd_display(int argc, char *argv[]) __reentrant
{
    uint32_t row = test_arg_u32(argc, argv, "row", 0);
    const char *text = test_arg_value(argc, argv, "text");
    const char *display_text;
    uint8_t column = 0u;
    drv_lcd1602_t lcd = DRV_LCD1602_INITIALIZER(
        BOARD_LCD1602_DATA_PORT,
        BOARD_LCD1602_RS_PIN,
        BOARD_LCD1602_RW_PIN,
        BOARD_LCD1602_ENABLE_PIN
    );
    
    if (!text) {
        text = "STC89C52RC CLI";
    }
    display_text = text;
    
    if (row > 1) row = 0;
    
    drv_lcd1602_init(&lcd);
    drv_lcd1602_clear(&lcd);
    drv_lcd1602_set_cursor(&lcd, (uint8_t)row, 0u);

    while ((*display_text != '\0') && (column < DRV_LCD1602_COLS)) {
        drv_lcd1602_write_char(&lcd, *display_text);
        display_text++;
        column++;
    }
    
    cli_printf("LCD: row=%u, text='%s'\r\n", (unsigned)row, text);
    
    return test_pass();
}
