# Architecture

The repository is split into four layers:

1. `src/app`: product or board-specific application code.
2. `src/drivers`: reusable drivers composed from HAL calls, such as LEDs, buttons, 16x2 LCDs, software I2C, and 74HC595 shift registers.
3. `src/hal`: direct STC89C52RC/8052 hardware access for GPIO, UART, timers, clock, and delays.
4. `src/common`: pure C utilities that are safe to unit test on the host.

The HAL owns all direct SFR access. Drivers should depend on HAL headers, not on `mcu.h`. Application code can use both HAL and drivers.

The STC89C52RC is 8051 compatible. The default configuration assumes classic 12T timing at 11.0592 MHz, which gives clean UART divisors for common baud rates. If your chip is configured for a different clock or machine-cycle mode, update `F_CPU` and `STC89C52RC_MACHINE_CYCLE_DIVIDER`.
