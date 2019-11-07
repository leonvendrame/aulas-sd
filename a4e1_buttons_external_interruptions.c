#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))

#define LEDS_DDR DDRD
#define LEDS_PORT PORTD

#define BTNS_DDR DDRD
#define BTNS_PORT PORTD
#define BTNS_PIN PIND

#define LED0 PD5
#define LED1 PD6
#define LED2 PD7

#define BTN0 PD2
#define BTN1 PD3
#define BTN2 PD4

#define INT_BTN0 PCINT18
#define INT_BTN1 PCINT19
#define INT_BTN2 PCINT20

ISR(PCINT2_vect) {
    if (!tst_bit(BTNS_PIN, BTN0)) {
        if (!tst_bit(LEDS_PORT, LED0)) {
            set_bit(LEDS_PORT, LED0);
        } else {
            clr_bit(LEDS_PORT, LED0);
        }
    } else if (!tst_bit(BTNS_PIN, BTN1)) {
        if (!tst_bit(LEDS_PORT, LED1)) {
            set_bit(LEDS_PORT, LED1);
        } else {
            clr_bit(LEDS_PORT, LED1);
        }
    } else if (!tst_bit(BTNS_PIN, BTN2)) {
        if (!tst_bit(LEDS_PORT, LED2)) {
            set_bit(LEDS_PORT, LED2);
        } else {
            clr_bit(LEDS_PORT, LED2);
        }
    }

    _delay_ms(15);
}

int main() {
    BTNS_DDR &= 0xE3;
    BTNS_PORT |= 0x1C;

    LEDS_DDR |= 0xE0;
    LEDS_PORT &= 0x1F;

    PCICR = 1 << PCIE2;

    PCMSK2 = (1 << INT_BTN0) | (1 << INT_BTN1) | (1 << INT_BTN2);

    sei();

    while (1);

    return 0;
}