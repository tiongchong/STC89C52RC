#include "stc89c52rc/cli/test_registry.h"
#include "stc89c52rc/cli/cli_print.h"
#include <string.h>

// Forward declarations of test functions
int test_gpio_write(int argc, char *argv[]) __reentrant;
int test_gpio_read(int argc, char *argv[]) __reentrant;
int test_gpio_toggle(int argc, char *argv[]) __reentrant;
int test_led_on(int argc, char *argv[]) __reentrant;
int test_led_off(int argc, char *argv[]) __reentrant;
int test_led_toggle(int argc, char *argv[]) __reentrant;
int test_led_blink(int argc, char *argv[]) __reentrant;
int test_button_read(int argc, char *argv[]) __reentrant;
int test_timer_start(int argc, char *argv[]) __reentrant;
int test_timer_read(int argc, char *argv[]) __reentrant;
int test_timer_delay(int argc, char *argv[]) __reentrant;
int test_uart_loopback(int argc, char *argv[]) __reentrant;
int test_crc8(int argc, char *argv[]) __reentrant;
int test_lcd_display(int argc, char *argv[]) __reentrant;
int test_soft_i2c_scan(int argc, char *argv[]) __reentrant;
int test_shift_reg_write(int argc, char *argv[]) __reentrant;

/**
 * Complete test registry - all available tests
 */
static const stc89c52rc_test_case_t test_registry[] = {
    // GPIO tests
    {"gpio.write", "Write GPIO pin (pin=<n> value=<0|1>)", test_gpio_write},
    {"gpio.read", "Read GPIO pin state (pin=<n>)", test_gpio_read},
    {"gpio.toggle", "Toggle GPIO pin (pin=<n> times=<n>)", test_gpio_toggle},
    
    // LED tests
    {"led.on", "Turn LED on", test_led_on},
    {"led.off", "Turn LED off", test_led_off},
    {"led.toggle", "Toggle LED", test_led_toggle},
    {"led.blink", "Blink LED (count=<n> ms=<n>)", test_led_blink},
    
    // Button tests
    {"button.read", "Read button state (poll=1 for continuous)", test_button_read},
    
    // Timer tests
    {"timer.start", "Start timer in mode=<16bit|8bit> (reload=<0xFFFF>)", test_timer_start},
    {"timer.read", "Read timer value", test_timer_read},
    {"timer.delay", "Test delay ms=<milliseconds>", test_timer_delay},
    
    // UART tests
    {"uart.loopback", "UART loopback test (send=<string>)", test_uart_loopback},
    
    // Checksum tests
    {"crc8.compute", "Compute CRC8 of data (data=<hex_string>)", test_crc8},
    
    // LCD tests
    {"lcd.display", "Display text on LCD (text=<string>)", test_lcd_display},
    
    // I2C tests
    {"i2c.scan", "Scan I2C bus for devices", test_soft_i2c_scan},
    
    // Shift register tests
    {"shiftreg.write", "Write to 74HC595 shift register (value=<0-255>)", test_shift_reg_write},
};

static const size_t test_count = sizeof(test_registry) / sizeof(test_registry[0]);

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
