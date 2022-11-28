#include "sys/irq.h"

#include "sys/plic.h"
#include "sys/printf.h"

void sys_irq_handler(void) {
    uint64_t mcause, mepc;
    uint32_t irq_cause;

    sys_irq_disable_all();

    __asm__ volatile(
        "csrr %0, mcause\n"
        "csrr %1, mepc\n"
        : "=r" (mcause), "=r" (mepc)
        :: "memory"
    );

    if(mcause >> 63) {
        irq_cause = (mcause & 0xff);
        switch(irq_cause) {
            case 11:
                plic_handle_irq();
                break;
            default:
                printf("unknown irq cause=0x%llx\n", irq_cause);
        }
    }
    else {
        printf("trap: mcause=0x%llx, mepc=0x%llx\n", mcause, mepc);
        sys_delay(1000 * 1000);
    }

    sys_irq_enable_all();
}
