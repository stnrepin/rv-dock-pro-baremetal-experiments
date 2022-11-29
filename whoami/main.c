#include "sys/kernel.h"
#include "sys/lradc.h"
#include "sys/sysdef.h"
#include "sys/uart.h"

#include "art.h"

enum states : int {
    STATE_A = 0,
    STATE_B = 1,
    STATE_C = 2,
    STATE_D = 3,
    STATE_END = 4,
};

enum states cur_state;
const char * const diags[] = {
    [STATE_A] = "- Who am I?\n",
    [STATE_B] = "- You are Rei Ayanami.\n",
    [STATE_C] = "- But, who are you? Are you Rei Ayanami as well?\n",
    [STATE_D] = ("- Correct, I’m the thing that is recognized as Rei Ayanami. "
                    "We are all things that are recognized as Rei Ayanami.\n")
};

static void handler(sys_ev_id_t ev) {
    switch (ev) {
        case SYS_EV_INIT:
            cur_state = STATE_A;
            sys_uart_puts("\n");
            break;

        case SYS_EV_BUT0_UP:
            sys_uart_puts(diags[cur_state]);
            cur_state++;
            break;
    }

    if (cur_state == STATE_END) {
        sys_uart_puts("\n\n");
        sys_uart_puts(art_read());
        return;
    }

    sys_add_handler(handler, SYS_EV_BUT0_UP);
}

int main(void) {
    sys_init();
    sys_init_handler(handler);
    return sys_run();
}
