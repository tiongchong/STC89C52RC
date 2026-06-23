#include <stc89c52rc/drivers/lcd1602.h>
#include <stc89c52rc/compiler.h>
#include <stc89c52rc/hal/delay.h>

#define LCD_CMD_CLEAR 0x01u
#define LCD_CMD_HOME 0x02u
#define LCD_CMD_ENTRY_MODE 0x06u
#define LCD_CMD_DISPLAY_OFF 0x08u
#define LCD_CMD_DISPLAY_ON 0x0cu
#define LCD_CMD_FUNCTION_8BIT_2LINE 0x38u
#define LCD_CMD_SET_DDRAM 0x80u

static STC_IDATA char lcd_lines[DRV_LCD1602_ROWS][DRV_LCD1602_LINE_BYTES];
static STC_IDATA char lcd_current[DRV_LCD1602_LINE_BYTES];
static STC_IDATA uint8_t lcd_current_len;
static STC_IDATA uint8_t lcd_committed_lines;

static void lcd_clear_line(char STC_IDATA *line)
{
    uint8_t index;

    for (index = 0u; index < DRV_LCD1602_LINE_BYTES; index++) {
        line[index] = '\0';
    }
}

static void lcd_copy_line(char STC_IDATA *destination, const char STC_IDATA *source)
{
    uint8_t index;

    for (index = 0u; index < DRV_LCD1602_LINE_BYTES; index++) {
        destination[index] = source[index];
    }
}

static void lcd_pulse_enable(const drv_lcd1602_t *lcd)
{
    hal_gpio_write(&lcd->enable, true);
    hal_delay_us(1u);
    hal_gpio_write(&lcd->enable, false);
    hal_delay_us(1u);
}

static void lcd_write_byte(const drv_lcd1602_t *lcd, uint8_t rs, uint8_t value)
{
    hal_gpio_write(&lcd->rs, rs != 0u);
    hal_gpio_write(&lcd->rw, false);
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

static void lcd_render_line(const drv_lcd1602_t *lcd, uint8_t row, const char STC_IDATA *text)
{
    uint8_t index;
    char value;

    drv_lcd1602_set_cursor(lcd, row, 0u);
    for (index = 0u; index < DRV_LCD1602_COLS; index++) {
        value = text[index];
        drv_lcd1602_write_char(lcd, (value == '\0') ? ' ' : value);
    }
}

static void lcd_render_blank_line(const drv_lcd1602_t *lcd, uint8_t row)
{
    uint8_t index;

    drv_lcd1602_set_cursor(lcd, row, 0u);
    for (index = 0u; index < DRV_LCD1602_COLS; index++) {
        drv_lcd1602_write_char(lcd, ' ');
    }
}

static void lcd_refresh(const drv_lcd1602_t *lcd)
{
    if (lcd_committed_lines == 0u) {
        lcd_render_line(lcd, 0u, lcd_current);
        lcd_render_blank_line(lcd, 1u);
    } else if (lcd_committed_lines == 1u) {
        lcd_render_line(lcd, 0u, lcd_lines[0]);
        lcd_render_line(lcd, 1u, lcd_current);
    } else if (lcd_current_len != 0u) {
        lcd_render_line(lcd, 0u, lcd_lines[1]);
        lcd_render_line(lcd, 1u, lcd_current);
    } else {
        lcd_render_line(lcd, 0u, lcd_lines[0]);
        lcd_render_line(lcd, 1u, lcd_lines[1]);
    }
}

static void lcd_commit_current(drv_lcd1602_t *lcd)
{
    lcd_current[lcd_current_len] = '\0';

    if (lcd_committed_lines == 0u) {
        lcd_copy_line(lcd_lines[0], lcd_current);
        lcd_committed_lines = 1u;
    } else if (lcd_committed_lines == 1u) {
        lcd_copy_line(lcd_lines[1], lcd_current);
        lcd_committed_lines = 2u;
    } else {
        lcd_copy_line(lcd_lines[0], lcd_lines[1]);
        lcd_copy_line(lcd_lines[1], lcd_current);
    }

    lcd_clear_line(lcd_current);
    lcd_current_len = 0u;
    lcd_refresh(lcd);
}

void drv_lcd1602_init(drv_lcd1602_t *lcd)
{
    if (lcd == 0) {
        return;
    }

    lcd_clear_line(lcd_lines[0]);
    lcd_clear_line(lcd_lines[1]);
    lcd_clear_line(lcd_current);
    lcd_current_len = 0u;
    lcd_committed_lines = 0u;

    hal_gpio_write(&lcd->rs, false);
    hal_gpio_write(&lcd->rw, false);
    hal_gpio_write(&lcd->enable, false);

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
    lcd_clear_line(lcd_lines[0]);
    lcd_clear_line(lcd_lines[1]);
    lcd_clear_line(lcd_current);
    lcd_current_len = 0u;
    lcd_committed_lines = 0u;
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
        lcd_commit_current(lcd);
        return;
    }

    if (lcd_current_len >= DRV_LCD1602_COLS) {
        lcd_commit_current(lcd);
    }

    lcd_current[lcd_current_len++] = value;
    lcd_current[lcd_current_len] = '\0';
    lcd_refresh(lcd);
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
