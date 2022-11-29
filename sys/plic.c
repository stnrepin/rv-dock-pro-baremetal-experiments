#include "sys/plic.h"

#define PLIC_PHY_ADDR (0x10000000)

#define PLIC_PRIO_REG(n) (0x0000 + n*0x0004)
#define PLIC_MIE_REG(n) (0x2000 + n*0x0004)
#define PLIC_MCLAIM_REG (0x200004)

#define PLIC_MIP_MEIP (1 << 11)

#define PLIC_MIN_PRIO (1)

static plic_handler_t *g_handlers[PLIC_IRQ_COUNT];

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

void plic_set_handler(enum plic_irq irq, plic_handler_t h) {
    g_handlers[irq] = h;
    plic_enable(irq);
}

void plic_enable(enum plic_irq irq_num)
{
    uint32_t enable_reg_cnt, num, temp, write_val;
    enable_reg_cnt = irq_num / 32;
    num = irq_num % 32;

    write32(PLIC_PHY_ADDR + PLIC_PRIO_REG(irq_num), PLIC_MIN_PRIO);

    temp = read32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt));
    write_val = temp | (1 << num);
    write32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt), write_val);

    csr_mie_set(PLIC_MIP_MEIP);
}

void plic_disable(enum plic_irq irq_num)
{
    uint32_t enable_reg_cnt, num, temp, write_val;
    enable_reg_cnt = irq_num / 32;
    num = irq_num % 32;

    temp = read32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt));
    write_val = temp & (~(1 << num));
    write32(PLIC_PHY_ADDR + PLIC_MIE_REG(enable_reg_cnt), write_val);
}

static uint8_t plic_mclaim_get(void)
{
    return (uint8_t)read32(PLIC_PHY_ADDR + PLIC_MCLAIM_REG);
}

static void plic_mclaim_complete(int id)
{
    write32(PLIC_PHY_ADDR + PLIC_MCLAIM_REG, id);
}

void plic_handle_irq(void)
{
    enum plic_irq irq;
    plic_handler_t *h;

    csr_mie_clear(PLIC_MIP_MEIP);

    while ((irq = plic_mclaim_get())) {
        plic_mclaim_complete(irq);
        h = g_handlers[(uint8_t)irq];
        if (h != NULL) {
            h();
        }
    }

    csr_mie_set(PLIC_MIP_MEIP);
}
