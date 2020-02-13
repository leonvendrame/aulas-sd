#include <util/delay.h>
#include "dht11lib.h"
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) ((Y & (1 << bit_x)) >> bit_x)
#define HIGH 1
#define LOW 0

uint16_t expect_pulse(uint8_t level) {
    uint16_t count = 0;
    while (tst_bit(SENSOR_PIN, SENSOR_IO_PIN) != level && count < 16000) {
        count++;
    }
    return count;
}

uint8_t checksum(uint8_t temps[]) {
    uint8_t sum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        sum += temps[i];
    }
    if (temps[4] == sum) {
        return 1;
    }
    return 0;
}

uint8_t read_temperature(uint8_t read_temp[5]) {
    uint8_t count = 0, bitwise_count = 0;
    uint16_t time_count[2] = {0, 0};

    set_bit(SENSOR_DDR, SENSOR_IO_PIN);

    set_bit(SENSOR_PORT, SENSOR_IO_PIN);
    _delay_us(1);

    clr_bit(SENSOR_PORT, SENSOR_IO_PIN);
    _delay_ms(18);

    set_bit(SENSOR_PORT, SENSOR_IO_PIN);
    clr_bit(SENSOR_DDR, SENSOR_IO_PIN);
    set_bit(SENSOR_PORT, SENSOR_IO_PIN);
    if (expect_pulse(LOW) >= 16000) {
        return 0;
    }

    if (expect_pulse(HIGH) >= 16000) {
        return 0;
    }
    if (expect_pulse(LOW) >= 16000) {
        return 0;
    }

    while (count < 5) {
        while (bitwise_count < 8) {
            read_temp[count] <<= 1;
            time_count[0] = expect_pulse(HIGH);
            time_count[1] = expect_pulse(LOW);
            if (time_count[0] >= 16000 || time_count[1] >= 16000) { 
                return 0; 
            }
            if (time_count[0] > time_count[1]) {
                read_temp[count] |= 0;
            } else {
                read_temp[count] |= 1;
            }
            bitwise_count++;
        }
        bitwise_count = 0;
        count++;
    }
    return 1;
}