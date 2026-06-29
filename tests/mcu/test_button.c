#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/hal/gpio.h"
#include "stc89c52rc/hal/delay.h"

/**
 * Button read test - Read current button state
 * Usage: test button.read [poll=1 samples=<n>]
 */
int test_button_read(int argc, char *argv[]) __reentrant
{
    uint8_t poll = test_arg_bool(argc, argv, "poll", 0);
    uint32_t samples = test_arg_u32(argc, argv, "samples", 10);
    
    // Button is on P3.2, active low
    hal_gpio_pin_t button_pin = {3, 2};
    
    if (poll) {
        cli_printf("Polling button %u times (100ms interval)...\r\n", (unsigned)samples);
        uint32_t i;
        for (i = 0; i < samples; i++) {
            uint8_t raw_state = hal_gpio_read(&button_pin);
            uint8_t pressed = !raw_state;  // Active low
            cli_printf("Sample %u: %s\r\n", (unsigned)(i+1), pressed ? "PRESSED" : "RELEASED");
            hal_delay_ms(100);
        }
    } else {
        uint8_t raw_state = hal_gpio_read(&button_pin);
        uint8_t pressed = !raw_state;  // Active low
        cli_printf("Button is %s\r\n", pressed ? "PRESSED" : "RELEASED");
    }
    
    return test_pass();
}
