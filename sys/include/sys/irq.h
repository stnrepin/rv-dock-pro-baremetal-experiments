#ifndef SYS_IRQ_H
#define SYS_IRQ_H

#include "sys/sysdef.h"

extern void sys_irq_enable_all();
extern void sys_irq_disable_all();

__attribute__((interrupt("machine")))
__attribute__((aligned(16)))
        void sys_irq_handler(void);

#endif // !SYS_IRQ_H
