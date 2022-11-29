#ifndef SYS_PLIT_H
#define SYS_PLIT_H

#include "sys/sysdef.h"

#define PLIC_IRQ_COUNT (256)

enum plic_irq : uint8_t {
    PLIC_IRQ_LRADC = 77
};

typedef void(plic_handler_t)();

void plic_set_handler(enum plic_irq irq, plic_handler_t f);

void plic_enable(enum plic_irq irq_num);
void plic_disable(enum plic_irq irq_num);
void plic_handle_irq(void);

#endif // !SYS_PLIT_H
