#include "sys/kernel.h"
#include "sys/uart.h"

enum user_event : sys_ev_id_t {
    USER_EVENT_A = SYS_USER_MIN_EV,
    USER_EVENT_B,
};

static void handler(sys_ev_id_t ev) {
    switch (ev) {
        case SYS_EV_INIT:
            sys_uart_puts("INIT\n");
            ev = USER_EVENT_A;
            break;
        case USER_EVENT_A:
            sys_uart_puts("A\n");
            sys_add_handler(handler, USER_EVENT_B);
            ev = USER_EVENT_B;
            break;
        case USER_EVENT_B:
            sys_uart_puts("B\n");
            ev = USER_EVENT_A;
            break;
    }

    sys_delay(1000 * 1000);
    sys_add_handler(handler, ev);
    sys_post(ev);
}

int main(void) {
    sys_init();
    sys_uart_puts("Hello, D1!\n");

    sys_init_handler(handler);

    return sys_run();
}
