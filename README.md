# STC89C52RC Firmware Repository

This repository is a starter implementation for STC89C52RC firmware using SDCC for compilation and stcgal for flashing. It is organized around a small hardware abstraction layer, reusable board drivers, and a host test suite for code that can be validated without an 8051 simulator.

## Requirements

- GNU Make
- SDCC with `sdcc` and `packihx` on `PATH`
- Python 3, used by `stcgal` and the cross-platform Make helpers
- stcgal, usually installed with `python -m pip install stcgal`

Windows users can run the Makefile from MSYS2, Git Bash, or another GNU Make environment. Keep SDCC, Python, and stcgal visible in `PATH`.

## Common Commands

```sh
make
make test
make flash PORT=/dev/ttyUSB0
make flash PORT=/dev/cu.usbserial-0001
make flash PORT=COM3
make clean
```

The Makefile auto-selects a default serial port per OS:

- Windows: `COM3`
- Linux: `/dev/ttyUSB0`
- macOS: `/dev/cu.usbserial-0001`

Override any of these at the command line:

```sh
make F_CPU=12000000UL UART_BAUD=9600UL
make flash PORT=COM14 STCGAL_BAUD=115200
```

## Layout

```text
include/stc89c52rc/
  board.h                 Board pin defaults for the demo app
  compiler.h              Compiler portability helpers
  config.h                Clock, UART, and device defaults
  mcu.h                   SDCC 8052 SFR include boundary
  common/                 Host-testable utility headers
  drivers/                Board driver interfaces
  hal/                    Hardware abstraction interfaces
src/
  app/main.c              Example firmware entry point
  common/                 Utilities shared by firmware and tests
  drivers/                GPIO-backed reusable drivers
  hal/                    STC89C52RC/8052 HAL implementation
tests/host/               Native C test suite
docs/                     Architecture and toolchain notes
```

The demo firmware initializes UART, toggles a status LED, and reports button state. Default pins are in `include/stc89c52rc/board.h`; change those to match your board before flashing.
