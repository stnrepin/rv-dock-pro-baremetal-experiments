#include "sys/kernel.h"

#include "sys/irq.h"
#include "sys/lradc.h"
#include "sys/mbox.h"
#include "sys/sysdef.h"
#include "sys/uart.h"

struct sys_handlers g_handlers;
struct sys_mbox_prios g_mboxes;

static void call_handler(sys_ev_id_t ev);

typedef void(irq_handler_f)();
extern void sys_irq_set_handler(irq_handler_f f);

void sys_init(void) {
    sys_mbox_new_handlers(&g_handlers);
    sys_mbox_new_mbox_prio(&g_mboxes);

    sys_uart_init();
    sys_lradc_init();

    sys_irq_set_handler(sys_irq_handler);
    sys_irq_enable_all();
}

void sys_init_handler(sys_handler_t h) {
    h(SYS_EV_INIT);
}

void sys_add_handler(sys_handler_t h, sys_ev_id_t target) {
    g_handlers.hs[target] = h;
}

void sys_post(sys_ev_id_t ev) {
    sys_post_prio(ev, SYS_PRIO_NORMAL);
}

void sys_post_prio(sys_ev_id_t ev, sys_prio_t prio) {
    sys_mbox_push(&g_mboxes, ev, prio);
}

int sys_run(void) {
    sys_ev_id_t ev;

    while (1) {
        if (sys_mbox_empty(&g_mboxes)) {
            continue;
        }

        ev = sys_mbox_pop(&g_mboxes, SYS_PRIO_NORMAL);
        if (ev == SYS_EV_NOP) {
            continue;
        }
        call_handler(ev);
    }

    return 0;
}

void call_handler(sys_ev_id_t ev) {
    // disable irq?
    sys_handler_t *h;

    h = g_handlers.hs[ev];
    if (h != NULL) {
        g_handlers.hs[ev] = NULL;
        h(ev);
    }
}

void _putchar(char c) {
    sys_uart_putc(c);
}
