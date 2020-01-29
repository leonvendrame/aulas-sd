#include<avr/io.h>
#include<util/delay.h>
#include<ky013lib.h>
#include<uart.h>
#include<stdio.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define cpl_bit(Y, bit_x) ()

double average(double temperatures[], uint8_t samples_number) {
    double sum = 0;
    for (uint8_t i = 0; i < samples_number; i++) {
        sum += temperatures[i];
    }

    return sum / samples_number;
}

int main() {
    double temp_read[3];

    uart_init();
    temp_sensor_init();

    while (1) {
        temp_read[0] = read_temperature(0, 0);
        temp_read[1] = read_temperature(0, 0);
        temp_read[2] = read_temperature(0, 0);
        
        printf("%.2lf \n", average(temp_read, 3));

        _delay_ms(1000);
    }

    return 0;
}