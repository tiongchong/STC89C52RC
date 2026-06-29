# STC89C52RC CLI Test Framework

This document describes how to use the new CLI-based test framework for the STC89C52RC microcontroller, modeled after the FE310's testing architecture.

## Overview

The CLI (Command-Line Interface) test system allows interactive testing of hardware modules via UART serial connection. Instead of flashing separate test firmware for each module, you can now:

1. Flash a single CLI-enabled firmware
2. Connect via serial terminal (9600 baud)
3. Run individual tests interactively
4. Parameterize tests with key=value arguments
5. Get immediate feedback on test results

## Architecture

### Directory Structure

```
src/cli/                      # CLI implementation
├── cli_commands.c           # Command dispatcher & handlers
├── cli_print.c              # Safe printf over UART
└── test_utils.c             # Argument parsing helpers

include/stc89c52rc/cli/       # CLI headers
├── cli_commands.h
├── cli_print.h
├── test_registry.h          # Test registry interface
└── test_utils.h

tests/mcu/                    # MCU-based hardware tests
├── test_gpio.c
├── test_led.c
├── test_button.c
├── test_timer.c
├── test_uart.c
├── test_crc8.c
├── test_lcd.c
├── test_soft_i2c.c
└── test_shift_register.c
```

### Key Components

1. **CLI Commands** - Command dispatcher that routes user input to test functions
2. **Test Registry** - Compile-time registry of all available tests
3. **Test Utils** - Helper functions for argument parsing (key=value format)
4. **CLI Print** - Safe printf-based output (128-byte buffer, no malloc)

## Building & Flashing

```bash
# Build the firmware (requires SDCC)
cd STC89C52RC
make firmware

# Flash to device (requires stcgal)
make flash PORT=/dev/ttyUSB0
```

## Using the CLI

### Serial Connection

Connect to the STC89C52RC at 9600 baud (8N1):

```bash
# Linux/macOS
screen /dev/ttyUSB0 9600

# or with pyserial
python3 -c "import serial; s=serial.Serial('/dev/ttyUSB0', 9600); s.interactive()"
```

### Available Commands

```
help                    - Show available commands
info                    - Display system information
test list               - List all available tests
test <name> [args]      - Run a specific test with optional arguments
```

### Command Examples

#### List All Tests

```
STC89C52RC> test list
Available tests (18):
  gpio.write           - Write GPIO pin (pin=<n> value=<0|1>)
  gpio.read            - Read GPIO pin state (pin=<n>)
  gpio.toggle          - Toggle GPIO pin (pin=<n> times=<n>)
  led.on               - Turn LED on
  led.off              - Turn LED off
  led.toggle           - Toggle LED
  led.blink            - Blink LED (count=<n> ms=<n>)
  button.read          - Read button state (poll=1 for continuous)
  timer.start          - Start timer in mode=<16bit|8bit>
  timer.read           - Read timer value
  timer.delay          - Test delay ms=<milliseconds>
  uart.loopback        - UART loopback test (send=<string>)
  crc8.compute         - Compute CRC8 of data (data=<hex_string>)
  lcd.display          - Display text on LCD (text=<string>)
  i2c.scan             - Scan I2C bus for devices
  shiftreg.write       - Write to 74HC595 shift register (value=<0-255>)
```

#### GPIO Tests

```
# Write GPIO high
STC89C52RC> test gpio.write pin=0 value=1
PASS gpio.write rc=0

# Read GPIO state
STC89C52RC> test gpio.read pin=0
GPIO P3.2 = LOW (raw=0)
PASS gpio.read rc=0

# Toggle GPIO 10 times with 50ms delay
STC89C52RC> test gpio.toggle times=10 delay_ms=50
Toggling P2.0 10 times, 50 ms delay
..........
PASS gpio.toggle rc=0
```

#### LED Tests

```
# Blink LED 5 times with 100ms toggles
STC89C52RC> test led.blink count=5 ms=100
Blinking LED 5 times (100 ms per toggle)
.....
PASS led.blink rc=0
```

#### Button Tests

```
# Read button state once
STC89C52RC> test button.read
Button is RELEASED
PASS button.read rc=0

# Poll button state 20 times
STC89C52RC> test button.read poll=1 samples=20
Polling button 20 times (100ms interval)
Sample 1: RELEASED
Sample 2: RELEASED
...
PASS button.read rc=0
```

#### Timer Tests

```
# Start Timer 0 in 16-bit mode
STC89C52RC> test timer.start mode=16bit
Starting Timer 0 in 16bit mode
Timer 0 started in 16-bit mode
PASS timer.start rc=0

# Read timer value (3 samples, 100ms apart)
STC89C52RC> test timer.read samples=3
Reading Timer 0 (3 samples):
  Sample 1: 0x1234 (4660)
  Sample 2: 0x2468 (9320)
  Sample 3: 0x36A1 (13985)
PASS timer.read rc=0

# Test 500ms delay
STC89C52RC> test timer.delay ms=500 repeat=2
Testing delay: 500 ms, repeated 2 times
  Delay 1 starting...
  Done
  Delay 2 starting...
  Done
PASS timer.delay rc=0
```

#### CRC8 Tests

```
# Compute CRC8 of test data
STC89C52RC> test crc8.compute
CRC8 of [01 02 03 04 05]: 0x45
Bulk CRC: 0x45, Incremental CRC: 0x45
PASS crc8.compute rc=0
```

#### LCD Tests

```
# Display text on LCD
STC89C52RC> test lcd.display row=0 text=Hello
LCD: row=0, text='Hello'
PASS lcd.display rc=0
```

## Argument Format

Tests use `key=value` format for arguments. Examples:

```
test gpio.write pin=5 value=1
test led.blink count=10 ms=100
test timer.delay ms=500 repeat=3
test button.read poll=1 samples=20
```

### Supported Value Types

- **Numeric**: Decimal or hex (0x prefix)
  - `value=10` or `value=0x0A`
- **Boolean**: "1", "true", "yes", "on" for true; "0", "false", "no", "off" for false
  - `poll=1` or `poll=true` or `poll=yes`
- **String**: Any text without spaces
  - `text=HelloWorld` (spaces not supported in this version)

## Test Output

Each test returns:
- **PASS** - Test completed successfully (rc=0)
- **FAIL** - Test detected an error (rc=-1)
- **Diagnostic output** - Via `cli_printf()` during test execution

## Adding New Tests

To add a new hardware test:

### 1. Create Test Function

Create `tests/mcu/test_<module>.c`:

```c
#include "stc89c52rc/cli/cli_print.h"
#include "stc89c52rc/cli/test_utils.h"
#include "stc89c52rc/drivers/<module>.h"

int test_module_function(int argc, char **argv)
{
    // Parse arguments with defaults
    uint32_t param = test_arg_u32(argc, argv, "param", 10);
    bool flag = test_arg_bool(argc, argv, "flag", false);
    
    // Test implementation
    cli_printf("Testing with param=%u, flag=%s\r\n", 
              (unsigned)param, flag ? "true" : "false");
    
    // Return result
    return test_pass();  // or test_fail()
}
```

### 2. Register Test

Add forward declaration and entry to test registry in `src/cli/test_registry.c`:

```c
// Forward declaration
int test_module_function(int argc, char **argv);

// Add to test_registry array:
static const stc89c52rc_test_case_t test_registry[] = {
    // ... existing entries ...
    {"module.function", "Description of what test does", test_module_function},
};
```

### 3. Rebuild

```bash
make firmware
```

## Performance Considerations

- **CLI Buffer**: 80-character command buffer
- **Printf Buffer**: 128-byte output buffer (safely truncates)
- **Max Arguments**: 10 per command
- **MCU Resources**: Minimal overhead - tests run inline, no background tasks

## Troubleshooting

### No CLI Prompt Appears

- Check UART connection and baud rate (9600)
- Verify `hal_uart_init()` completes successfully
- LCD should show "CLI Ready" at startup

### Test Not Found

```
Unknown command: test bad.name
```

Run `test list` to verify correct test name.

### Argument Parsing Issues

- Arguments must be in `key=value` format (no spaces around `=`)
- Single argument values only (no quoted strings with spaces in this version)
- Check default values in test implementation

### Memory Issues

If firmware size exceeds 8KB:
- Remove unused drivers from includes
- Use more efficient string formatting
- Consider moving test logic to host-based tests (see tests/host/)

## Architecture Comparison with FE310

| Feature | FE310 | STC89C52RC |
|---------|-------|-----------|
| **CLI Library** | embedded-cli | Custom lightweight CLI |
| **Output Method** | embeddedCliPrint() | UART direct |
| **Command Parsing** | embedded-cli tokenizer | Simple string parsing |
| **Max Arguments** | Configurable | 10 fixed |
| **Printf Buffer** | 192 bytes | 128 bytes |
| **UART Baud** | 115200 | 9600 |
| **Target** | 32-bit RISC-V | 8-bit 8051 |

## Files Created

### Headers
- `include/stc89c52rc/cli/cli_print.h` - Printf interface
- `include/stc89c52rc/cli/cli_commands.h` - CLI initialization
- `include/stc89c52rc/cli/test_registry.h` - Test registry interface
- `include/stc89c52rc/cli/test_utils.h` - Argument parsing helpers

### Implementation
- `src/cli/cli_print.c` - Safe printf implementation
- `src/cli/cli_commands.c` - Command dispatcher
- `src/cli/test_registry.c` - Test registry and lookup
- `src/cli/test_utils.c` - Argument parsing

### Tests
- `tests/mcu/test_gpio.c` - GPIO tests
- `tests/mcu/test_led.c` - LED tests
- `tests/mcu/test_button.c` - Button tests
- `tests/mcu/test_timer.c` - Timer tests
- `tests/mcu/test_uart.c` - UART loopback tests
- `tests/mcu/test_crc8.c` - Checksum tests
- `tests/mcu/test_lcd.c` - LCD display tests
- `tests/mcu/test_soft_i2c.c` - I2C bus tests
- `tests/mcu/test_shift_register.c` - 74HC595 tests

### Modified Files
- `src/app/main.c` - Integrated CLI startup and polling loop
- `Makefile` - Updated to include CLI and MCU test sources

---

**Next Steps**: Connect to your STC89C52RC over UART and start running tests!

For detailed hardware specifications, see [docs/](../docs/).
