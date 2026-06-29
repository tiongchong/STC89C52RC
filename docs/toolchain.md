# Toolchain Notes

## Build

`make` compiles the demo app, HAL, CLI, and only the drivers required by the selected firmware image:

```sh
make
```

Build products are written to `build/`. SDCC emits an Intel HEX-like `.ihx` file, and `packihx` converts it to `.hex`.

On-chip CLI tests are opt-in so the default image can fit in the STC89C52RC's 8 KB flash and 256 bytes of internal RAM:

```sh
make MCU_TESTS="gpio led button"
make MCU_TESTS=all
```

`MCU_TESTS=all` may exceed the device limits; build a small set of tests when flashing real hardware.

## Flash

stcgal talks to the STC bootloader over a USB-to-UART adapter:

```sh
make flash PORT=/dev/ttyUSB0
make flash PORT=/dev/cu.usbserial-0001
make flash PORT=COM3
```

Typical wiring:

- Adapter TX to MCU RXD/P3.0
- Adapter RX to MCU TXD/P3.1
- Common GND
- Power-cycle or reset the chip when stcgal is waiting for the bootloader

The default protocol is `stc89`. Override `STCGAL_PROTOCOL`, `STCGAL_BAUD`, or `STCGAL_FLAGS` if your adapter or bootloader needs different settings.

## Tests

Host tests are GoogleTest programs and do not require SDCC:

```sh
make test
```

Keep hardware-free utilities in `src/common` so they can be covered by host tests. The GoogleTest suite is not intended to run on the STC89C52RC itself; use a small SDCC hardware-in-loop firmware for on-chip tests.
