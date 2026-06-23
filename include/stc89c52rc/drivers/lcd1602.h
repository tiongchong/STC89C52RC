#ifndef STC89C52RC_DRIVERS_LCD1602_H
#define STC89C52RC_DRIVERS_LCD1602_H

#include <stdint.h>
#include <stc89c52rc/hal/gpio.h>

#define DRV_LCD1602_ROWS 2u
#define DRV_LCD1602_COLS 16u
#define DRV_LCD1602_LINE_BYTES (DRV_LCD1602_COLS + 1u)

typedef struct {
    uint8_t data_port;
    hal_gpio_pin_t rs;
    hal_gpio_pin_t rw;
    hal_gpio_pin_t enable;
} drv_lcd1602_t;

#define DRV_LCD1602_INITIALIZER(data_port_, rs_, rw_, enable_) \
    { (uint8_t)(data_port_), rs_, rw_, enable_ }

#ifdef __cplusplus
extern "C" {
#endif

void drv_lcd1602_init(drv_lcd1602_t *lcd);
void drv_lcd1602_clear(drv_lcd1602_t *lcd);
void drv_lcd1602_set_cursor(const drv_lcd1602_t *lcd, uint8_t row, uint8_t column);
void drv_lcd1602_write_char(const drv_lcd1602_t *lcd, char value);

void drv_lcd1602_putc(drv_lcd1602_t *lcd, char value);
void drv_lcd1602_puts(drv_lcd1602_t *lcd, const char *text);
void drv_lcd1602_println(drv_lcd1602_t *lcd, const char *text);
void drv_lcd1602_put_uint16(drv_lcd1602_t *lcd, uint16_t value);
void drv_lcd1602_put_hex8(drv_lcd1602_t *lcd, uint8_t value);

#ifdef __cplusplus
}
#endif

#endif
