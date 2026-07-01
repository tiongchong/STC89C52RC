#include <stc89c52rc/drivers/lcd1602.h>
#include <stc89c52rc/hal/delay.h>

#define LCD_CMD_CLEAR 0x01u
#define LCD_CMD_HOME 0x02u
#define LCD_CMD_ENTRY_MODE 0x06u
#define LCD_CMD_DISPLAY_OFF 0x08u
#define LCD_CMD_DISPLAY_ON 0x0cu
#define LCD_CMD_FUNCTION_8BIT_2LINE 0x38u
#define LCD_CMD_SET_DDRAM 0x80u

static uint8_t lcd_cursor_row;
static uint8_t lcd_cursor_col;

static void lcd_pulse_enable(const drv_lcd1602_t *lcd)
{
    hal_gpio_write(&lcd->enable, 1u);
    hal_delay_us(1u);
    hal_gpio_write(&lcd->enable, 0u);
    hal_delay_us(1u);
}

static void lcd_write_byte(const drv_lcd1602_t *lcd, uint8_t rs, uint8_t value)
{
    hal_gpio_write(&lcd->rs, rs != 0u);
    hal_gpio_write(&lcd->rw, 0u);
    hal_gpio_set_port(lcd->data_port, value);
    lcd_pulse_enable(lcd);
}

static void lcd_command(const drv_lcd1602_t *lcd, uint8_t command)
{
    lcd_write_byte(lcd, 0u, command);

    if ((command == LCD_CMD_CLEAR) || (command == LCD_CMD_HOME)) {
        hal_delay_ms(2u);
    } else {
        hal_delay_us(50u);
    }
}


void drv_lcd1602_init(drv_lcd1602_t *lcd)
{
    if (lcd == 0) {
        return;
    }

    lcd_cursor_row = 0u;
    lcd_cursor_col = 0u;

    hal_gpio_write(&lcd->rs, 0u);
    hal_gpio_write(&lcd->rw, 0u);
    hal_gpio_write(&lcd->enable, 0u);

    hal_delay_ms(40u);
    lcd_command(lcd, 0x30u);
    hal_delay_ms(5u);
    lcd_command(lcd, 0x30u);
    hal_delay_us(150u);
    lcd_command(lcd, 0x30u);
    lcd_command(lcd, LCD_CMD_FUNCTION_8BIT_2LINE);
    lcd_command(lcd, LCD_CMD_DISPLAY_OFF);
    lcd_command(lcd, LCD_CMD_CLEAR);
    lcd_command(lcd, LCD_CMD_ENTRY_MODE);
    lcd_command(lcd, LCD_CMD_DISPLAY_ON);
}

void drv_lcd1602_clear(drv_lcd1602_t *lcd)
{
    if (lcd == 0) {
        return;
    }

    lcd_command(lcd, LCD_CMD_CLEAR);
    lcd_cursor_row = 0u;
    lcd_cursor_col = 0u;
}

void drv_lcd1602_set_cursor(const drv_lcd1602_t *lcd, uint8_t row, uint8_t column)
{
    uint8_t address;

    if (lcd == 0) {
        return;
    }

    if (row >= DRV_LCD1602_ROWS) {
        row = (uint8_t)(DRV_LCD1602_ROWS - 1u);
    }
    if (column >= DRV_LCD1602_COLS) {
        column = (uint8_t)(DRV_LCD1602_COLS - 1u);
    }

    lcd_cursor_row = row;
    lcd_cursor_col = column;

    address = (row == 0u) ? column : (uint8_t)(0x40u + column);
    lcd_command(lcd, (uint8_t)(LCD_CMD_SET_DDRAM | address));
}

void drv_lcd1602_write_char(const drv_lcd1602_t *lcd, char value)
{
    if (lcd == 0) {
        return;
    }

    lcd_write_byte(lcd, 1u, (uint8_t)value);
    hal_delay_us(50u);
}

void drv_lcd1602_putc(drv_lcd1602_t *lcd, char value)
{
    if (lcd == 0) {
        return;
    }

    if (value == '\r') {
        return;
    }

    if (value == '\n') {
        lcd_cursor_row = (uint8_t)((lcd_cursor_row + 1u) % DRV_LCD1602_ROWS);
        lcd_cursor_col = 0u;
        drv_lcd1602_set_cursor(lcd, lcd_cursor_row, lcd_cursor_col);
        return;
    }

    if (lcd_cursor_col >= DRV_LCD1602_COLS) {
        lcd_cursor_row = (uint8_t)((lcd_cursor_row + 1u) % DRV_LCD1602_ROWS);
        lcd_cursor_col = 0u;
        drv_lcd1602_set_cursor(lcd, lcd_cursor_row, lcd_cursor_col);
    }

    drv_lcd1602_write_char(lcd, value);
    lcd_cursor_col++;
}

void drv_lcd1602_puts(drv_lcd1602_t *lcd, const char *text)
{
    while ((lcd != 0) && (text != 0) && (*text != '\0')) {
        drv_lcd1602_putc(lcd, *text);
        text++;
    }
}

void drv_lcd1602_println(drv_lcd1602_t *lcd, const char *text)
{
    drv_lcd1602_puts(lcd, text);
    drv_lcd1602_putc(lcd, '\n');
}

void drv_lcd1602_put_uint16(drv_lcd1602_t *lcd, uint16_t value)
{
    uint16_t divisor = 10000u;
    uint8_t digit;

    while ((divisor > value) && (divisor > 1u)) {
        divisor = (uint16_t)(divisor / 10u);
    }

    while (divisor != 0u) {
        digit = (uint8_t)(value / divisor);
        drv_lcd1602_putc(lcd, (char)('0' + digit));
        value = (uint16_t)(value % divisor);
        divisor = (uint16_t)(divisor / 10u);
    }
}

static char lcd_hex_digit(uint8_t value)
{
    value = (uint8_t)(value & 0x0fu);
    if (value < 10u) {
        return (char)('0' + value);
    }

    return (char)('A' + value - 10u);
}

void drv_lcd1602_put_hex8(drv_lcd1602_t *lcd, uint8_t value)
{
    drv_lcd1602_putc(lcd, lcd_hex_digit((uint8_t)(value >> 4u)));
    drv_lcd1602_putc(lcd, lcd_hex_digit(value));
}
