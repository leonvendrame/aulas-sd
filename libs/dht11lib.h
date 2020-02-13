#include <avr/io.h>
#define SENSOR_IO_PIN PB0
#define SENSOR_PIN PINB
#define SENSOR_PORT PORTB
#define SENSOR_DDR DDRB

uint16_t expect_pulse(uint8_t level);

uint8_t checksum(uint8_t temps[]);

uint8_t read_temperature(uint8_t read_temp[]);