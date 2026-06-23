# Toolchain Notes

## Build

`make` compiles every C file below `src/` with SDCC:

```sh
make
```

Build products are written to `build/`. SDCC emits an Intel HEX-like `.ihx` file, and `packihx` converts it to `.hex`.

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

Host tests are native C programs and do not require SDCC:

```sh
make test
```

Keep hardware-free utilities in `src/common` so they can be covered by host tests.
