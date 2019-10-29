#include <avr/io.h>
#include <util/delay.h>
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) ((Y &= (1 << bit_x)) >> bit_x)
#define HIGH 1
#define LOW 0
#define SENSOR_IO_PIN PD2
#define SENSOR_PIN PIND
#define SENSOR_PORT PORTD
#define SENSOR_DDR DDRD

uint16_t expect_pulse(uint8_t level) {
	uint16_t count = 0;
	while (tst_bit(SENSOR_PIN, SENSOR_IO_PIN) != level || count > 16000) {
		count++;
	}
	return count;
}

void checksum() {
	//TODO checksum()
}

void read_temperature(uint8_t read_temp[]) {
	uint8_t count = 5, bit_count = 8;
	uint8_t time_count[2] = {0, 0};

	set_bit(SENSOR_DDR, SENSOR_IO_PIN);

	set_bit(SENSOR_PORT, SENSOR_IO_PIN);
	_delay_us(1);

	clr_bit(SENSOR_PORT, SENSOR_IO_PIN);
	_delay_us(18);

	set_bit(SENSOR_PORT, SENSOR_IO_PIN);
	clr_bit(SENSOR_DDR, SENSOR_IO_PIN);
	set_bit(SENSOR_PORT, SENSOR_IO_PIN);
    expect_pulse(0);

	expect_pulse(1); //80us
	expect_pulse(0); //80us

	while(count > 0) {
        while (bit_count > 0) {
            *read_temp[bit_count] << 1;
            time_count[0] = expect_pulse(1);
            time_count[1] = expect_pulse(0);
            if (time_count[0] > time_count[1]) {
	    		*read_temp[bit_count] |= 0;
			} else {
                *read_temp[bit_count] |= 1;
                bit_count--;
		    }
            bit_count = 8;
            count--;
        }
	}
}

int main() {
	uint8_t *temps, aux;
	read_temperature(temps);
	for (uint8_t i = 0; i < 5; i++) {
		aux = temps[i];
	}
}