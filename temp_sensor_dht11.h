#include <avr/io.h>
#define SENSOR_IO_PIN PD2
#define SENSOR_PIN PIND
#define SENSOR_PORT PORTD
#define SENSOR_DDR DDRD

uint16_t expect_pulse(uint8_t level);

uint8_t checksum(uint8_t temps[]);

uint8_t read_temperature(uint8_t read_temp[]);