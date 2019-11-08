#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))

#define LEDS_DDR DDRD
#define LEDS_PORT PORTD

#define BTNS_DDR DDRD
#define BTNS_PORT PORTD
#define BTNS_PIN PIND

#define LED0 PD5

#define BTN0 PD2
#define BTN1 PD3

#define INT_BTN0 PCINT18
#define INT_BTN1 PCINT19

ISR(INT1_vect) {
    cpl_bit(LEDS_PORT, LED0);
    _delay_ms(300);
}

ISR(INT0_vect) {
    for (uint8_t i = 0; i < 12; i++) {
        cpl_bit(LEDS_PORT, LED0);
        _delay_ms(50);
    }
}

int main() {
    BTNS_DDR &= (~(1 << BTN0) & ~(1 << BTN1));
    BTNS_PORT |= ((1 << BTN0) | (1 << BTN1));

    LEDS_DDR |= (1 << LED0);
    LEDS_PORT &= (~(1 << LED0));

    EICRA = 1 << ISC01;

    EIMSK = (1 << INT1) | (1 << INT0);

    sei();

    while (1);

    return 0;
}