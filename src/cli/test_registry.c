#include "stc89c52rc/cli/test_registry.h"
#include "stc89c52rc/cli/cli_print.h"
#include <string.h>

#if defined(STC89C52RC_ENABLE_TEST_GPIO)
int test_gpio_write(int argc, char *argv[]) __reentrant;
int test_gpio_read(int argc, char *argv[]) __reentrant;
int test_gpio_toggle(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_LED)
int test_led_on(int argc, char *argv[]) __reentrant;
int test_led_off(int argc, char *argv[]) __reentrant;
int test_led_toggle(int argc, char *argv[]) __reentrant;
int test_led_blink(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_BUTTON)
int test_button_read(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_TIMER)
int test_timer_start(int argc, char *argv[]) __reentrant;
int test_timer_read(int argc, char *argv[]) __reentrant;
int test_timer_delay(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_UART)
int test_uart_loopback(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_CRC8)
int test_crc8(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_LCD)
int test_lcd_display(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_SOFT_I2C)
int test_soft_i2c_scan(int argc, char *argv[]) __reentrant;
#endif
#if defined(STC89C52RC_ENABLE_TEST_SHIFT_REGISTER)
int test_shift_reg_write(int argc, char *argv[]) __reentrant;
#endif

/**
 * Test registry - populated from the MCU_TESTS build option.
 */
static const stc89c52rc_test_case_t test_registry[] = {
#if defined(STC89C52RC_ENABLE_TEST_GPIO)
    {"gpio.write", "Write GPIO pin (pin=<n> value=<0|1>)", test_gpio_write},
    {"gpio.read", "Read GPIO pin state (pin=<n>)", test_gpio_read},
    {"gpio.toggle", "Toggle GPIO pin (pin=<n> times=<n>)", test_gpio_toggle},
#endif
#if defined(STC89C52RC_ENABLE_TEST_LED)
    {"led.on", "Turn LED on", test_led_on},
    {"led.off", "Turn LED off", test_led_off},
    {"led.toggle", "Toggle LED", test_led_toggle},
    {"led.blink", "Blink LED (count=<n> ms=<n>)", test_led_blink},
#endif
#if defined(STC89C52RC_ENABLE_TEST_BUTTON)
    {"button.read", "Read button state (poll=1 for continuous)", test_button_read},
#endif
#if defined(STC89C52RC_ENABLE_TEST_TIMER)
    {"timer.start", "Start timer in mode=<16bit|8bit> (reload=<0xFFFF>)", test_timer_start},
    {"timer.read", "Read timer value", test_timer_read},
    {"timer.delay", "Test delay ms=<milliseconds>", test_timer_delay},
#endif
#if defined(STC89C52RC_ENABLE_TEST_UART)
    {"uart.loopback", "UART loopback test (send=<string>)", test_uart_loopback},
#endif
#if defined(STC89C52RC_ENABLE_TEST_CRC8)
    {"crc8.compute", "Compute CRC8 of data (data=<hex_string>)", test_crc8},
#endif
#if defined(STC89C52RC_ENABLE_TEST_LCD)
    {"lcd.display", "Display text on LCD (text=<string>)", test_lcd_display},
#endif
#if defined(STC89C52RC_ENABLE_TEST_SOFT_I2C)
    {"i2c.scan", "Scan I2C bus for devices", test_soft_i2c_scan},
#endif
#if defined(STC89C52RC_ENABLE_TEST_SHIFT_REGISTER)
    {"shiftreg.write", "Write to 74HC595 shift register (value=<0-255>)", test_shift_reg_write},
#endif
    {0, 0, 0}
};

static const size_t test_count = (sizeof(test_registry) / sizeof(test_registry[0])) - 1u;

const stc89c52rc_test_case_t *stc89c52rc_test_registry(uint32_t *count)
{
    if (count) {
        *count = (uint32_t)test_count;
    }
    return test_registry;
}

const stc89c52rc_test_case_t *stc89c52rc_test_find(const char *name)
{
    if (!name) return NULL;
    
    for (size_t i = 0; i < test_count; i++) {
        if (strcmp(test_registry[i].name, name) == 0) {
            return &test_registry[i];
        }
    }
    
    return NULL;
}
