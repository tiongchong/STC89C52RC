#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/board.h"
#include "stc89c52rc/drivers/led.h"
#include "stc89c52rc/hal/delay.h"

static drv_led_t status_led = { BOARD_STATUS_LED_PIN, DRV_LED_ACTIVE_HIGH };

/**
 * LED on test - Turn on the LED
 * Usage: test led.on
 */
int test_led_on(int argc, char *argv[]) __reentrant
{
    (void)argc;
    (void)argv;
    
    drv_led_on(&status_led);
    cli_puts("LED turned ON\r\n");
    
    return test_pass();
}

/**
 * LED off test - Turn off the LED
 * Usage: test led.off
 */
int test_led_off(int argc, char *argv[]) __reentrant
{
    (void)argc;
    (void)argv;
    
    drv_led_off(&status_led);
    cli_puts("LED turned OFF\r\n");
    
    return test_pass();
}

/**
 * LED toggle test - Toggle the LED once
 * Usage: test led.toggle
 */
int test_led_toggle(int argc, char *argv[]) __reentrant
{
    (void)argc;
    (void)argv;
    
    drv_led_toggle(&status_led);
    cli_puts("LED toggled\r\n");
    
    return test_pass();
}

/**
 * LED blink test - Blink LED multiple times
 * Usage: test led.blink count=<n> ms=<milliseconds>
 */
int test_led_blink(int argc, char *argv[]) __reentrant
{
    uint32_t count = test_arg_u32(argc, argv, "count", 5);
    uint32_t ms = test_arg_u32(argc, argv, "ms", 100);
    
    cli_printf("Blinking LED %u times (%u ms per toggle)\r\n", (unsigned)count, (unsigned)ms);
    
    for (uint32_t i = 0; i < count; i++) {
        drv_led_toggle(&status_led);
        hal_delay_ms(ms);
        cli_puts(".");
    }
    cli_putln();
    
    drv_led_off(&status_led);  // Leave LED off
    
    return test_pass();
}
