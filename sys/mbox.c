#include "sys/mbox.h"
#include "sys/uart.h"

void sys_mbox_new_handlers(struct sys_handlers *hs) {
    int i;

    for (i = 0; i < SYS_EVENT_TYPE_COUNT; i++) {
        hs->hs[i] = NULL;
    }
}

void sys_mbox_new_mbox_prio(struct sys_mbox_prios *ms) {
    int i;

    ms->mbox_emptiness = 0;
    for (i = 0; i < SYS_PRIO_COUNT; i++) {
        ms->prio_mbox->end_pos = 0;
        ms->prio_mbox->top_pos = 0;
        ms->prio_mbox->size = 0;
    }
}

void sys_mbox_push(struct sys_mbox_prios *m, sys_ev_id_t ev, sys_prio_t prio) {
    struct sys_mbox *mb;
    mb = &m->prio_mbox[prio];

    mb->events[mb->end_pos++] = ev;
    if (mb->end_pos == SYS_MBOX_SIZE) {
        mb->end_pos = 0;
    }
    mb->size++;
    m->mbox_emptiness |= (1 << prio);
}

sys_ev_id_t sys_mbox_pop(struct sys_mbox_prios *m, sys_prio_t prio) {
    struct sys_mbox *mb;
    sys_ev_id_t ev;
    mb = &m->prio_mbox[prio];

    ev = mb->events[mb->top_pos++];
    if (mb->top_pos == SYS_MBOX_SIZE) {
        mb->top_pos = 0;
    }
    mb->size--;
    if (mb->size == 0) {
        m->mbox_emptiness &= ~(1 << prio);
    }
    return ev;
}
