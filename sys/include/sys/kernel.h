#ifndef SYS_KERNEL_H
#define SYS_KERNEL_H

#include "sys/mbox.h"
#include "sys/config.h"

#define SYS_PRIO_NORMAL (SYS_PRIO_COUNT / 2)

enum sys_event : sys_ev_id_t {
    SYS_EV_NOP,
    SYS_EV_INIT,
    SYS_EV_BUT0_UP,

    SYS_USER_MIN_EV,
};

void sys_init(void);

void sys_init_handler(sys_handler_t handler);
void sys_add_handler(sys_handler_t handler, sys_ev_id_t target);

void sys_post(sys_ev_id_t ev);
void sys_post_prio(sys_ev_id_t ev, sys_prio_t prio);

int sys_run(void);

#endif // !SYS_KERNEL_H
