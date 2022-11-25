#include "sys/lradc.h"

#define LRADC_BASE (0x02009800)

#define LRADC_BGR_REG (0x2001a9c)
#define LRADC_CTRL (LRADC_BASE + 0)
#define LRADC_INTC (LRADC_BASE + 0x0004)
#define LRADC_DATA (LRADC_BASE + 0x000c)

void sys_lradc_init(void) {
    u32_t val;

    // Configure LRADC_BGR_REG[LRADC_RST] to 1 to deassert the reset of LRADC
    val = 0x10000;
    write32(LRADC_BGR_REG, val);

    // Configure LRADC_BGR_REG[LRADC_GATING] to 1 to enable the clock of LRADC.
    val = read32(LRADC_BGR_REG);
    val |= 0x1;
    write32(LRADC_BGR_REG, val);

    // Configure LRADC_CTRL[LRADC_HOLD_KEY_EN] to 1.
    val = read32(LRADC_CTRL);
    val |= 0x80;
    write32(LRADC_CTRL, val);

    // Configure LRADC_INTC to enable the corresponding interrupt.
    //
    val = 0x10; // ADC0_KEYUP_EN
    write32(LRADC_INTC, val);

    // Configure LRADC_CTRL[LRADC_EN] to 1
    val = read32(LRADC_CTRL);
    val |= 0x1;
    write32(LRADC_CTRL, val);
}

u8_t sys_lradc_data(void) {
    return read32(LRADC_DATA);
}
