# Tests

The `host` suite builds with the native C compiler and exercises pure C modules from `src/common`.

Run:

```sh
make test
```

Hardware-facing HAL and driver modules are intentionally kept thin. Add hardware-in-loop smoke tests separately once a board fixture and serial port convention exist.
