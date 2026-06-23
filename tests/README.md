# Tests

The `host` suite uses GoogleTest and exercises pure C modules from `src/common`.

Run:

```sh
make test
```

The Makefile links with `gtest_main`. If `pkg-config` can find GoogleTest, it uses those flags; otherwise it falls back to `-lgtest_main -lgtest -pthread`.

## Real Hardware

GoogleTest should be treated as a host-side test framework for this project. It is not a good fit to run directly on the STC89C52RC because the chip is an 8051-class MCU with limited RAM/flash, the firmware is built with SDCC C, and GoogleTest expects a C++ runtime, heap, standard library, and host process model.

For real hardware, use a separate hardware-in-loop suite:

1. Build and flash a small SDCC test firmware with `make flash`.
2. Emit compact pass/fail records over UART.
3. Drive the board from a host script that uses stcgal and a serial port.

That keeps rich assertions on the host while the chip runs a tiny purpose-built test firmware.
