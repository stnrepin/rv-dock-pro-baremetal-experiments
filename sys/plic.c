#include "sys/plic.h"

#include "sys/uart.h"
#include "sys/printf.h"
#include "sys/lradc.h"

#define PLIC_PHY_ADDR (0x10000000)

#define PLIC_PRIO_REG(n) (0x0000 + n*0x0004)
#define PLIC_MIE_REG(n) (0x2000 + n*0x0004)
#define PLIC_MCLAIM_REG (0x200004)

#define PLIC_MIP_MEIP (1 << 11)

#define PLIC_MIN_PRIO (1)

static inline void csr_mie_set(uint64_t val) {
    __asm__ volatile(
            "csrs mie, %0\n"
            :: "rK" (val) : "memory"
    );
}

static inline void csr_mie_clear(uint64_t val) {
    __asm__ volatile(
            "csrc mie, %0\n"
            :: "rK" (val) : "memory"
    );
}

void plic_enable(uint32_t irq_num)
{
    int enable_reg_cnt = irq_num / 32;
    int num = irq_num % 32;

    write32(PLIC_PHY_ADDR + PLIC_PRIO_REG(irq_num), PLIC_MIN_PRIO);

    int temp = read32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt));
    int write_val = temp | (1 << num);
    write32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt), write_val);

    csr_mie_set(PLIC_MIP_MEIP);
}

void plic_disable(uint32_t irq_num)
{
    int enable_reg_cnt = irq_num / 32;
    int num = irq_num % 32;

    int temp = read32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt));
    int write_val = temp & (~(1 << num));
    write32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt), write_val);
}

static int plic_mclaim_get(void)
{
    return read32(PLIC_PHY_ADDR + PLIC_MCLAIM_REG);
}

__attribute__((used)) static void plic_mclaim_complete(int id)
{
    write32(PLIC_PHY_ADDR + PLIC_MCLAIM_REG, id);
}

void plic_handle_irq(void)
{
    unsigned int irq;

    csr_mie_clear(PLIC_MIP_MEIP);

    while ((irq = plic_mclaim_get())) {
        plic_mclaim_complete(irq);
        sys_lradc_reset_irq();
    }

    csr_mie_set(PLIC_MIP_MEIP);
}
