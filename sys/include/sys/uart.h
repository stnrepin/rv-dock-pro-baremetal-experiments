#ifndef SYS_UART_H
#define SYS_UART_H

void sys_uart_init(void);
void sys_uart_putc(char c);
void sys_uart_puts(const char* s);

#endif // !SYS_UART_H
