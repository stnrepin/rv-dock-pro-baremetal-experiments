#include "sys/kernel.h"
#include "sys/uart.h"

int main() {
    sys_init();
    sys_uart_puts("Hello, D1!\n");

    while(1);

    return 0;
}
