#ifndef SYS_MBOX_H
#define SYS_MBOX_H

#include "sys/sysdef.h"
#include "sys/config.h"

typedef uint8_t sys_ev_id_t;
typedef uint8_t sys_prio_t;
typedef void (sys_handler_t)(sys_ev_id_t);

// TODO: Rewrite according rte_ring implementation from DPDK on atomics.
struct sys_mbox {
    volatile int top_pos;
    volatile int end_pos;
    volatile int size;
    volatile sys_ev_id_t events[SYS_MBOX_SIZE];
};

struct sys_mbox_prios {
    volatile uint32_t mbox_emptiness;
    struct sys_mbox prio_mbox[SYS_PRIO_COUNT];
};

struct sys_handlers {
    sys_handler_t *hs[SYS_EVENT_TYPE_COUNT];
};

void sys_mbox_new_handlers(struct sys_handlers *hs);
void sys_mbox_new_mbox_prio(struct sys_mbox_prios *ms);

void sys_mbox_push(struct sys_mbox_prios *m, sys_ev_id_t ev, sys_prio_t prio);
sys_ev_id_t sys_mbox_pop(struct sys_mbox_prios *m, sys_prio_t prio);

static inline int sys_mbox_empty(struct sys_mbox_prios *m) {
    return m->mbox_emptiness == 0;
}

#endif // !SYS_MBOX_H
