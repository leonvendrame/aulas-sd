#include <avr/io.h>
#include "uart.h"

#define LEDS_DDR DDRB
#define LEDS_PORT PORTB
#define LED0 PB0
#define LED1 PB1
#define LED2 PB2
#define LED3 PB3

#define PIN0 PD0
#define PIN1 PD1
#define PIN2 PD2
#define PIN3 PD3
#define PIN4 PD4
#define PIN5 PD5
#define PIN6 PD6
#define PIN7 PD7
#define PIN8 PB0
#define PIN9 PB1
#define PIN10 PB2
#define PIN11 PB3
#define PIN12 PB4
#define PIN13 PB5

int main(void) {
    int valor;

    uart_init();

    while (1) {
        
    }

    return 0;
}