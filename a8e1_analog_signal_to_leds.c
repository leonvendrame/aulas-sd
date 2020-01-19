#include<avr/io.h>
#include<util/delay.h>
#include<adclib.h>

#define LED0 PD7
#define LED1 PD6
#define LED2 PD5
#define LED3 PD4
#define LED4 PD3
#define LEDS_DDR DDRD
#define LEDS_PORT PORTD

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))

const char LEDS[] = {
    LED0, LED1, LED2, LED3, LED4
};

void leds_init() {
    LEDS_DDR |= ((1 << LED0) | (1 << LED1) | (1 << LED2) |\
        (1 << LED3) | (1 << LED4));
}

void switch_leds(uint8_t led_on) {
    for (uint8_t i = 0; i < 5; i++) {
        if (i == led_on) {
            set_bit(LEDS_PORT, LEDS[i]);
        } else {
            clr_bit(LEDS_PORT, LEDS[i]);
        }
    }
}

void led_multiplexer(uint16_t number) {
    if (number < 204) {
        switch_leds(4);
    } else if (number < 408) {
        switch_leds(3);
    } else if (number < 612) {
        switch_leds(2);
    } else if (number < 816) {
        switch_leds(1);
    } else {
        switch_leds(0);
    }
}

int main() {
    uint16_t read;

    leds_init();
    adc_init();

    while (1) {
        read = adc_read(0);
        led_multiplexer(read);
        _delay_ms(50);
    }

    return 0;
}