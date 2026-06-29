#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/hal/timer.h"
#include "stc89c52rc/hal/delay.h"

/**
 * Timer start test - Initialize and start Timer 0
 * Usage: test timer.start [mode=<16bit|8bit>] [reload=<value>]
 */
int test_timer_start(int argc, char *argv[]) __reentrant
{
    const char *mode_str = test_arg_value(argc, argv, "mode");
    
    if (!mode_str) {
        mode_str = "16bit";
    }
    
    cli_printf("Starting Timer 0 in %s mode\r\n", mode_str);
    
    if (mode_str[0] == '8') {
        // 8-bit mode
        cli_puts("8-bit mode (not fully implemented)\r\n");
    } else {
        // 16-bit mode
        hal_timer0_configure_16bit();
        hal_timer0_start();
        cli_puts("Timer 0 started in 16-bit mode\r\n");
    }
    
    return test_pass();
}

/**
 * Timer read test - Read the current value of Timer 0
 * Usage: test timer.read [samples=<n>]
 */
int test_timer_read(int argc, char *argv[]) __reentrant
{
    uint32_t samples = test_arg_u32(argc, argv, "samples", 3);
    
    cli_printf("Reading Timer 0 (%u samples):\r\n", (unsigned)samples);
    
    for (uint32_t i = 0; i < samples; i++) {
        uint16_t count = hal_timer0_read();
        cli_printf("  Sample %u: 0x%04X (%u)\r\n", (unsigned)(i+1), count, count);
        hal_delay_ms(100);
    }
    
    return test_pass();
}

/**
 * Timer delay test - Test the delay functionality
 * Usage: test timer.delay [ms=<milliseconds>] [repeat=<times>]
 */
int test_timer_delay(int argc, char *argv[]) __reentrant
{
    uint32_t ms = test_arg_u32(argc, argv, "ms", 500);
    uint32_t repeat = test_arg_u32(argc, argv, "repeat", 3);
    
    cli_printf("Testing delay: %u ms, repeated %u times\r\n", (unsigned)ms, (unsigned)repeat);
    
    for (uint32_t i = 0; i < repeat; i++) {
        cli_printf("  Delay %u starting...\r\n", (unsigned)(i+1));
        hal_delay_ms(ms);
        cli_puts("  Done\r\n");
    }
    
    return test_pass();
}
