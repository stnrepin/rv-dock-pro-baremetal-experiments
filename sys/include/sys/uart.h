#ifndef UART_H
#define UART_H

void sys_uart_init(void);
void sys_uart_putc(char c);
void sys_uart_puts(const char* s);

#endif // !UART_H
