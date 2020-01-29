#include<avr/io.h>
#include<math.h>

#define RESISTOR1 10000

#define CONST_A 1.129148e-3
#define CONST_B 2.34125e-4
#define CONST_C 8.76741e-8

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

void temp_sensor_init() {
    adc_init();
}

double calc_termistor_resistance(double resistor, uint16_t adc_value) {
    return resistor / ((1023.0 / adc_value) - 1);
}

double convert_kelvin_to_celsius(double kelvin_temp) {
    return kelvin_temp - 273;
}

double convert_kelvin_to_fahrenheit(double kelvin_temp) {
    return ((kelvin_temp - 273) * 9.0 / 5.0 + 32);
}

double read_kelvin_temperature(double termistor_resistance) {
    return 1.0 / (CONST_A + CONST_B * log(termistor_resistance) \
        + CONST_C * pow(log(termistor_resistance), 3));
}

double read_temperature(uint8_t pin, uint8_t scale) {
    uint16_t adc_value = adc_read(pin);

    double termistor_resistance = calc_termistor_resistance(RESISTOR1, adc_value);
    double kelvin_temp = read_kelvin_temperature(termistor_resistance);

    switch (scale) {
    case (0):
        return convert_kelvin_to_celsius(kelvin_temp);
        break;

    case (1):
        return convert_kelvin_to_celsius(kelvin_temp);
        break;

    case (2):
        return kelvin_temp;
        break;

    default:
        return convert_kelvin_to_celsius(kelvin_temp);
        break;
    }
}

