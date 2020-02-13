#include "uartlib.h"

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/setbaud.h>

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
  
    UCSR0A &= ~(_BV(U2X0));
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

int uart_putchar(char c) {
    if (c == '\n') {
        uart_putchar('\r');
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int uart_getchar() {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}


/* Get a string from serial and stores in str */
void getstring(char *str) {
    char c;
    while ((c=uart_getchar()) != '\n') {   
        *str=c;
        str++;
    }
    *str = '\0';
}

/* Put the string str to serial */
void putstring(char *str) {
    while (*str != '\0') {
        uart_putchar(*str);
        str++;
    }
}
