#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define BAUD 9600

void uart_init(void);
int uart_putchar(char c);
int uart_getchar(void);
void getstring(char *str);
void putstring(char *str);

#endif /* UART_H_ */
