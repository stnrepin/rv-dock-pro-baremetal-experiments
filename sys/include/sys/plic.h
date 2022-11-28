#ifndef SYS_PLIT_H
#define SYS_PLIT_H

#include "sys/sysdef.h"

#define PLIC_LRADC 77

void plic_enable(uint32_t irq_num);
void plic_disable(uint32_t irq_num);
void plic_handle_irq(void);

#endif // !SYS_PLIT_H
