#ifndef SYS_LRADC_H
#define SYS_LRADC_H

#include "sys/sysdef.h"

void sys_lradc_init(void);
void sys_lradc_reset_irq(void);
uint8_t sys_lradc_data(void);

#endif // !SYS_LRADC_H
