#include "sys/kernel.h"

#include "sys/uart.h"
#include "sys/lradc.h"

void sys_init(void) {
    sys_uart_init();
    sys_lradc_init();
}
