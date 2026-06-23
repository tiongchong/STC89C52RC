#include <stc89c52rc/drivers/lcd1602.h>
#include <stc89c52rc/hal/delay.h>

#define LCD_CMD_CLEAR 0x01u
#define LCD_CMD_HOME 0x02u
#define LCD_CMD_ENTRY_MODE 0x06u
#define LCD_CMD_DISPLAY_OFF 0x08u
#define LCD_CMD_DISPLAY_ON 0x0cu
#define LCD_CMD_FUNCTION_8BIT_2LINE 0x38u
#define LCD_CMD_SET_DDRAM 0x80u

static void lcd_clear_line(char *line)
{
    uint8_t index;

    for (index = 0u; index < DRV_LCD1602_LINE_BYTES; index++) {
        line[index] = '\0';
    }
}

static void lcd_copy_line(char *destination, const char *source)
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

static void lcd_render_line(const drv_lcd1602_t *lcd, uint8_t row, const char *text)
{
    uint8_t index;
    char value;

    drv_lcd1602_set_cursor(lcd, row, 0u);
    for (index = 0u; index < DRV_LCD1602_COLS; index++) {
        value = text[index];
        drv_lcd1602_write_char(lcd, (value == '\0') ? ' ' : value);
    }
}

static void lcd_refresh(const drv_lcd1602_t *lcd)
{
    static const char empty[DRV_LCD1602_LINE_BYTES] = { 0 };

    if (lcd->committed_lines == 0u) {
        lcd_render_line(lcd, 0u, lcd->current);
        lcd_render_line(lcd, 1u, empty);
    } else if (lcd->committed_lines == 1u) {
        lcd_render_line(lcd, 0u, lcd->lines[0]);
        lcd_render_line(lcd, 1u, lcd->current);
    } else if (lcd->current_len != 0u) {
        lcd_render_line(lcd, 0u, lcd->lines[1]);
        lcd_render_line(lcd, 1u, lcd->current);
    } else {
        lcd_render_line(lcd, 0u, lcd->lines[0]);
        lcd_render_line(lcd, 1u, lcd->lines[1]);
    }
}

static void lcd_commit_current(drv_lcd1602_t *lcd)
{
    lcd->current[lcd->current_len] = '\0';

    if (lcd->committed_lines == 0u) {
        lcd_copy_line(lcd->lines[0], lcd->current);
        lcd->committed_lines = 1u;
    } else if (lcd->committed_lines == 1u) {
        lcd_copy_line(lcd->lines[1], lcd->current);
        lcd->committed_lines = 2u;
    } else {
        lcd_copy_line(lcd->lines[0], lcd->lines[1]);
        lcd_copy_line(lcd->lines[1], lcd->current);
    }

    lcd_clear_line(lcd->current);
    lcd->current_len = 0u;
    lcd_refresh(lcd);
}

void drv_lcd1602_init(drv_lcd1602_t *lcd)
{
    if (lcd == 0) {
        return;
    }

    lcd_clear_line(lcd->lines[0]);
    lcd_clear_line(lcd->lines[1]);
    lcd_clear_line(lcd->current);
    lcd->current_len = 0u;
    lcd->committed_lines = 0u;

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
    lcd_clear_line(lcd->lines[0]);
    lcd_clear_line(lcd->lines[1]);
    lcd_clear_line(lcd->current);
    lcd->current_len = 0u;
    lcd->committed_lines = 0u;
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

    if (lcd->current_len >= DRV_LCD1602_COLS) {
        lcd_commit_current(lcd);
    }

    lcd->current[lcd->current_len++] = value;
    lcd->current[lcd->current_len] = '\0';
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
    char text[6];
    uint8_t index = 0u;

    if (value == 0u) {
        drv_lcd1602_putc(lcd, '0');
        return;
    }

    while ((value != 0u) && (index < sizeof(text))) {
        text[index++] = (char)('0' + (value % 10u));
        value = (uint16_t)(value / 10u);
    }

    while (index != 0u) {
        drv_lcd1602_putc(lcd, text[--index]);
    }
}

void drv_lcd1602_put_hex8(drv_lcd1602_t *lcd, uint8_t value)
{
    static const char hex[] = "0123456789ABCDEF";

    drv_lcd1602_putc(lcd, hex[(value >> 4u) & 0x0fu]);
    drv_lcd1602_putc(lcd, hex[value & 0x0fu]);
}
