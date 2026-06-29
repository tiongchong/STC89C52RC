#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/drivers/button.h"
#include "stc89c52rc/hal/delay.h"

/**
 * Button read test - Read current button state
 * Usage: test button.read [poll=1 samples=<n>]
 */
int test_button_read(int argc, char *argv[]) __reentrant
{
    bool poll = test_arg_bool(argc, argv, "poll", false);
    uint32_t samples = test_arg_u32(argc, argv, "samples", 10);
    
    if (poll) {
        cli_printf("Polling button %u times (100ms interval)...\r\n", (unsigned)samples);
        for (uint32_t i = 0; i < samples; i++) {
            bool pressed = button_is_pressed();
            cli_printf("Sample %u: %s\r\n", (unsigned)(i+1), pressed ? "PRESSED" : "RELEASED");
            hal_delay_ms(100);
        }
    } else {
        bool pressed = button_is_pressed();
        cli_printf("Button is %s\r\n", pressed ? "PRESSED" : "RELEASED");
    }
    
    return test_pass();
}
