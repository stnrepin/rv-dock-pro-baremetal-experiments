# Lichee RV Dock Pro experiments

RV Dock PRO has a B702 behind two USB-C ports: the first for FEL (left) and
the second for UART (right).

Run `sudo minicom -D /dev/ttyACM0 -b 115200` for UART.

You need installed `clang`, `llvm-objcopy`, and [`xfel`](https://github.com/xboot/xfel).

Go to a source folder and:

1. Use `make bin` to create flashable binary file `$NAME.bin`.
2. Use `sudo make fel-exec` to write and execute the binary file.

## `who-am-i`

Uses UART to send messages and KEY button to iterate over the messages.
