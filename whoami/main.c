#include "sys/kernel.h"
#include "sys/lradc.h"
#include "sys/sysdef.h"
#include "sys/uart.h"

#include "art.h"

enum states : int {
    STATE_A = 0,
    STATE_B = 1,
    STATE_C = 2,
    STATE_D = 3,
    STATE_END = 4,
};

int main(void) {
    u8_t data;
    enum states cur_state = STATE_A;
    const char * const diags[] = {
        [STATE_A] = "- Who am I?\n",
        [STATE_B] = "- You are Rei Ayanami.\n",
        [STATE_C] = "- But, who are you? Are you Rei Ayanami as well?\n",
        [STATE_D] = ("- Correct, I’m the thing that is recognized as Rei Ayanami. "
                     "We are all things that are recognized as Rei Ayanami.\n")
    };

    sys_init();

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

        sys_delay(100 * 1000); // 100ms
    }

    sys_uart_puts("\n\n");
    sys_uart_puts(art_read());

    while(1);

    return 0;
}
