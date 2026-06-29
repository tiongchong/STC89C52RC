# STC89C52RC Firmware Repository

This repository is a starter implementation for STC89C52RC firmware using SDCC for compilation and stcgal for flashing. It is organized around a small hardware abstraction layer, reusable board drivers, and a host test suite for code that can be validated without an 8051 simulator.

## Requirements

- GNU Make
- SDCC with `sdcc` and `packihx` on `PATH`
- GoogleTest for the host test suite
- A native C/C++ compiler, such as GCC, Clang, or MinGW
- Python 3, used by `stcgal` and the cross-platform Make helpers
- stcgal, usually installed with `python -m pip install stcgal`

Windows users can run the Makefile from MSYS2, Git Bash, or another GNU Make environment. Keep SDCC, Python, and stcgal visible in `PATH`.

## Common Commands

```sh
make
make MCU_TESTS="gpio led button"
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
tests/host/               GoogleTest host test suite
docs/                     Architecture and toolchain notes
```

The default firmware keeps the linked image small enough for the 8 KB STC89C52RC by building the demo app and its required drivers only. Hardware CLI tests are opt-in:

```sh
make MCU_TESTS="gpio led button"
make MCU_TESTS=all
```

`MCU_TESTS=all` is useful while developing, but it may exceed the chip's flash or internal RAM. Default pins are in `include/stc89c52rc/board.h`; change those to match your board before flashing.

## Demo LCD Wiring

The 16x2 LCD driver assumes an HD44780-compatible module in 8-bit mode:

- `D0..D7` to `P0.0..P0.7`
- `RS` to `P3.5`
- `RW` to `P3.6`
- `E` to `P3.4`

The LCD print API keeps a two-line log: the first completed line appears on row 1, the next line appears on row 2, and each newer line shifts the previous row 2 content up to row 1. On classic 8051 hardware, `P0` is open-drain, so use pull-ups on the LCD data bus if your board or LCD module does not already provide them.


Usages example :

```
make
```

Compiles the firmware with sdcc and writes output under build/, including:
```
build/stc89c52rc_demo.ihx
build/stc89c52rc_demo.hex
```
Build selected on-chip CLI tests with:
```
make MCU_TESTS="gpio led timer"
```
Run the host test suite with:
```
make test
```
That builds the native test runner with your system C compiler and runs tests for the common modules.

Useful extras:
```
make clean
make print-config
make flash PORT=/dev/ttyUSB0
make flash PORT=/dev/cu.usbserial-0001
make flash PORT=COM3
```

Requirements:
* sdcc
* packihx
* GNU Make
* native C compiler for tests, such as cc, gcc, or clang
* stcgal only for flashing, not for tests
