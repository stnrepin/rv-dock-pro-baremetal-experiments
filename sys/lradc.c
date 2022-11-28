#include "sys/lradc.h"

#include "sys/printf.h"

#define LRADC_BASE (0x02009800)

#define LRADC_BGR_REG (0x2001a9c)
#define LRADC_CTRL (LRADC_BASE + 0x0000)
#define LRADC_INTC (LRADC_BASE + 0x0004)
#define LRADC_INTS (LRADC_BASE + 0x0008)
#define LRADC_DATA (LRADC_BASE + 0x000c)

#define LRADC_PLIC_BASE (0x10000000)
#define LRADC_PLIC_N (77)
#define LRADC_PLIC_MIE (0x2000 + LRADC_PLIC_N*0x0004)

/* LRADC_CTRL bits */
#define FIRST_CONVERT_DLY(x)	((x) << 24) /* 8 bits */
#define CHAN_SELECT(x)		((x) << 22) /* 2 bits */
#define CONTINUE_TIME_SEL(x)	((x) << 16) /* 4 bits */
#define KEY_MODE_SEL(x)		((x) << 12) /* 2 bits */
#define LEVELA_B_CNT(x)		((x) << 8)  /* 4 bits */
#define HOLD_KEY_EN(x)		((x) << 7)
#define HOLD_EN(x)		((x) << 6)
#define LEVELB_VOL(x)		((x) << 4)  /* 2 bits */
#define SAMPLE_RATE(x)		((x) << 2)  /* 2 bits */
#define ENABLE(x)		((x) << 0)

void sys_lradc_init(void) {
    uint32_t val;

    // Configure LRADC_BGR_REG[LRADC_RST] to 1 to deassert the reset of LRADC
    val = 0x10000;
    write32(LRADC_BGR_REG, val);

    // Configure LRADC_BGR_REG[LRADC_GATING] to 1 to enable the clock of LRADC.
    val = read32(LRADC_BGR_REG);
    val |= 0x1;
    write32(LRADC_BGR_REG, val);

    // Configure LRADC_CTRL.
    //
    // https://github.com/torvalds/linux/blob/bf82d38c91f857083f2d1b9770fa3df55db2ca3b/drivers/input/keyboard/sun4i-lradc-keys.c#L172
    val = FIRST_CONVERT_DLY(2) | LEVELA_B_CNT(1) | HOLD_EN(1) | SAMPLE_RATE(3) | ENABLE(1);
    write32(LRADC_CTRL, val);

    // Configure LRADC_INTC to enable the corresponding interrupt.
    //
    val = 0x10; // ADC0_KEYUP_IRQ
    write32(LRADC_INTC, val);
}

void sys_lradc_reset_irq(void) {
    uint32_t val = read32(LRADC_INTS);
    printf("but0 irq\n");
    write32(LRADC_INTS, val);
}

uint8_t sys_lradc_data(void) {
    return read32(LRADC_DATA);
}
