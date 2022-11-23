#include "sys/uart.h"
#include "sys/sysdef.h"

#include "art.h"

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

enum states {
    STATE_A = 0,
    STATE_B = 1,
    STATE_C = 2,
    STATE_D = 3,
    STATE_END = 4,
};

u64_t counter(void)
{
    u64_t cnt;
    __asm__ __volatile__("csrr %0, time\n" : "=r"(cnt) :: "memory");
    return cnt;
}

void sdelay(unsigned long us) {
    u64_t t1 = counter();
    u64_t t2 = t1 + us * 24;
    do {
    t1 = counter();
    } while(t2 >= t1);
}

int main(void)
{
    u8_t data;
    enum states cur_state = STATE_A;
    const char * const diags[] = {
        [STATE_A] = "- Who am I?\n",
        [STATE_B] = "- You are Rei Ayanami.\n",
        [STATE_C] = "- But, who are you? Are you Rei Ayanami as well?\n",
        [STATE_D] = ("- Correct, I’m the thing that is recognized as Rei Ayanami. "
                     "We are all things that are recognized as Rei Ayanami.\n")
    };

    sys_uart_init();
    sys_lradc_init();

    sys_uart_puts("\n");

    while(1) {
        data = sys_lradc_data();
        if (data == 0x09) {
            sys_uart_puts(diags[cur_state]);
            cur_state++;
            if (cur_state == STATE_END) {
                break;
            }
        }

        sdelay(100 * 1000); // 100ms
    }

    sys_uart_puts("\n\n");
    sys_uart_puts(art_read());

    while(1);

    return 0;
}
