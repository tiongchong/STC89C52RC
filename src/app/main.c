#include <stdint.h>
#include <stc89c52rc/board.h>
#include <stc89c52rc/compiler.h>
#include <stc89c52rc/drivers/button.h>
#include <stc89c52rc/drivers/lcd1602.h>
#include <stc89c52rc/drivers/led.h>
#include <stc89c52rc/hal/delay.h>
#include <stc89c52rc/hal/uart.h>
#include <stc89c52rc/cli/cli_commands.h>
#include <stc89c52rc/cli/cli_print.h>

void main(void)
{
    // Initialize UART first
    hal_uart_init(BOARD_UART_BAUD);
    
    // Initialize hardware modules
    drv_led_t status_led = { BOARD_STATUS_LED_PIN, DRV_LED_ACTIVE_HIGH };
    drv_button_t user_button = { BOARD_USER_BUTTON_PIN, DRV_BUTTON_ACTIVE_LOW };
    
    drv_led_init(&status_led, 0u);
    drv_button_init(&user_button);
    
    // Initialize LCD display
    drv_lcd1602_t lcd = DRV_LCD1602_INITIALIZER(
        BOARD_LCD1602_DATA_PORT,
        BOARD_LCD1602_RS_PIN,
        BOARD_LCD1602_RW_PIN,
        BOARD_LCD1602_ENABLE_PIN
    );
    drv_lcd1602_init(&lcd);
    
    // Display startup message
    drv_lcd1602_puts(&lcd, "STC89C52RC");
    drv_lcd1602_putc(&lcd, '\n');
    drv_lcd1602_puts(&lcd, "CLI Ready");
    
    // Initialize and start CLI
    cli_init();
    
    // Main CLI loop - process UART input and run tests
    while (1) {
        cli_poll();
        
        // Optional: blink LED to show system is alive
        drv_led_toggle(&status_led);
        hal_delay_ms(50u);
    }
}
