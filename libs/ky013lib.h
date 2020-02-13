#ifndef KY013LIB_H_
#define KY013LIB_H_

void temp_sensor_init();

float read_temperature(uint8_t pin, uint8_t scale);

#endif