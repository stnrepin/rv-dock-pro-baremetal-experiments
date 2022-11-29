# Lichee RV Dock Pro experiments

RV Dock PRO has a B702 behind two USB-C ports: the first for FEL (left) and
the second for UART (right).

Run `sudo minicom -D /dev/ttyACM0 -b 115200` for UART.

You need installed `clang`, `llvm-objcopy`, and [`xfel`](https://github.com/xboot/xfel).

Go to a root folder and:

1. Use `make bins` to create flashable binary files for each subproject.
2. Use `sudo make fel-exec NAME=<SUBPROJECT_NAME>` to write and execute the
   binary file via XFEL.

## `who-am-i`

Uses UART to send the messages and KEY button to iterate over the messages.

## `hello`

Not interesting. Used merely for tests during development.
