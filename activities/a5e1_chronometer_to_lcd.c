#include <avr/io.h>
#include <util/delay.h>
#include "../libs/displaylcdlib.h"
#include <avr/interrupt.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))

#define BTN0 PB0
#define BTN1 PB1
#define BTNS_DDR DDRB
#define BTNS_PORT PORTB
#define BTNS_PIN PINB

volatile uint8_t stopped = 1;
volatile uint8_t cents = 0;
volatile uint8_t seconds = 0;
volatile uint8_t minutes = 0;

void display_time(uint8_t minutes, uint8_t seconds, uint8_t cents) {
    set_cursor(1, 5);
    display_num(minutes);
    display_str(":");
    display_num(seconds);
    display_str(":");
    display_num(cents);
}

ISR(TIMER1_COMPA_vect) {
    ++cents;
    if (cents == 100) {
        ++seconds;
        cents = 0;
    }
    if (seconds == 60) {
        ++minutes;
        seconds = 0;
    }
    if (minutes == 60) {
        minutes = 0;
    }
}

ISR(PCINT0_vect) {
    if (!tst_bit(BTNS_PIN, BTN1)) {
        cents = 0;
        seconds = 0;
        minutes = 0;
        if (stopped) {
            display_time(minutes, seconds, cents);
        }
    } else if (!tst_bit(BTNS_PIN, BTN0)) {
        stopped = !stopped;
        if (stopped) {
            TCCR1B &= (~(1 << CS12));
        } else {
            TCCR1B |= 1 << CS12;
        }
    }
}

void setup_buttons() {
    BTNS_DDR &= ~((1 << BTN0) | (1 << BTN1));
    BTNS_PORT |= ((1 << BTN0) | (1 << BTN1));
    PCICR = 1 << PCIE0;
    PCIFR = 1 << PCIF0;
    PCMSK0 = ((1 << PCINT1) | (1 << PCINT0));
}

void setup_timer_counter_1() {
    OCR1A = 625;
    TIMSK1 = 1 << OCIE1A;
    TCCR1B = 1 << WGM12;
}

int main() {
    setup_display();
    setup_buttons();
    setup_timer_counter_1();

    initialize_display();
    display_time(minutes, seconds, cents);

    sei();

    while (1) {
        while (stopped);
        display_time(minutes, seconds, cents);
    }

    return 0;
}