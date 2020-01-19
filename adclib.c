#include<avr/io.h>
#include<adclib.h>

#define CHANNEL_0 0x00
#define CHANNEL_1 0x01
#define CHANNEL_2 0x02
#define CHANNEL_3 0x03
#define CHANNEL_4 0x04
#define CHANNEL_5 0x05

#define tst_bit(Y, bit_x) (Y & (1 << bit_x))

const uint8_t channels[] = {
    CHANNEL_0, CHANNEL_1,
    CHANNEL_2, CHANNEL_3,
    CHANNEL_4, CHANNEL_5
};

void adc_init() {
    ADMUX |= 1 << REFS0;
    ADCSRA |= ((1 << ADEN) | (0x07));
}

void wait_for_convertion() {
    while (tst_bit(ADCSRA, ADSC)) {};
}

uint16_t adc_read(uint8_t channel) {
    if (channel < 0 || channel > 5) {
        return 0;
    }
    ADMUX &= 0xF0;
    ADMUX |= channels[channel];
    ADCSRA |= 1 << ADSC;
    wait_for_convertion();
    return ADC;
}