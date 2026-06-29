#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/hal/gpio.h"
#include "stc89c52rc/hal/delay.h"

/**
 * GPIO write test - Write value to a GPIO pin
 * Usage: test gpio.write pin=<n> value=<0|1>
 */
int test_gpio_write(int argc, char *argv[]) __reentrant
{
    uint32_t pin = test_arg_u32(argc, argv, "pin", 0);
    uint32_t value = test_arg_u32(argc, argv, "value", 0);
    
    // For STC89C52RC, we'll use P2.0 as example (status LED)
    // In a real test, you'd configure this based on pin number
    struct hal_gpio_pin led_pin = {.port = 2, .bit = 0};
    
    hal_gpio_write(&led_pin, value != 0);
    cli_printf("GPIO P%u.%u = %u\r\n", led_pin.port, led_pin.bit, (value != 0));
    
    return test_pass();
}

/**
 * GPIO read test - Read the current state of a GPIO pin
 * Usage: test gpio.read pin=<n>
 */
int test_gpio_read(int argc, char *argv[]) __reentrant
{
    uint32_t pin = test_arg_u32(argc, argv, "pin", 0);
    
    // For STC89C52RC button is on P3.2 (active low)
    struct hal_gpio_pin button_pin = {.port = 3, .bit = 2};
    
    bool state = hal_gpio_read(&button_pin);
    cli_printf("GPIO P%u.%u = %s (raw=%u)\r\n", 
              button_pin.port, button_pin.bit,
              state ? "HIGH" : "LOW",
              state);
    
    return test_pass();
}

/**
 * GPIO toggle test - Toggle GPIO pin multiple times
 * Usage: test gpio.toggle pin=<n> times=<n> delay_ms=<n>
 */
int test_gpio_toggle(int argc, char *argv[]) __reentrant
{
    uint32_t times = test_arg_u32(argc, argv, "times", 10);
    uint32_t delay_ms = test_arg_u32(argc, argv, "delay_ms", 50);
    
    struct hal_gpio_pin led_pin = {.port = 2, .bit = 0};
    
    cli_printf("Toggling P%u.%u %u times, %u ms delay\r\n",
              led_pin.port, led_pin.bit, (unsigned)times, (unsigned)delay_ms);
    
    for (uint32_t i = 0; i < times; i++) {
        hal_gpio_toggle(&led_pin);
        hal_delay_ms(delay_ms);
        if (i % 2 == 0) {
            cli_puts(".");
        }
    }
    cli_putln();
    
    return test_pass();
}
