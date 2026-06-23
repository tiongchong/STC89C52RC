#include <stdint.h>
#include <stc89c52rc/board.h>
#include <stc89c52rc/drivers/button.h>
#include <stc89c52rc/drivers/led.h>
#include <stc89c52rc/hal/delay.h>
#include <stc89c52rc/hal/uart.h>

void main(void)
{
    drv_led_t status_led = { BOARD_STATUS_LED_PIN, DRV_LED_ACTIVE_HIGH };
    drv_button_t user_button = { BOARD_USER_BUTTON_PIN, DRV_BUTTON_ACTIVE_LOW };
    uint16_t tick = 0u;

    hal_uart_init(BOARD_UART_BAUD);
    drv_led_init(&status_led, false);
    drv_button_init(&user_button);

    hal_uart_puts("\nSTC89C52RC firmware ready\n");

    while (1) {
        drv_led_toggle(&status_led);

        hal_uart_puts("tick=");
        hal_uart_put_uint16(tick++);
        hal_uart_puts(" button=");
        hal_uart_puts(drv_button_is_pressed(&user_button) ? "pressed\n" : "released\n");

        hal_delay_ms(500u);
    }
}
