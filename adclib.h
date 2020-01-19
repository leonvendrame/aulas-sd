#ifndef adclib_h_
#define adclib_h_

#include<avr/io.h>

void adc_init();

void wait_for_convertion();

uint16_t adc_read(uint8_t channel);

#endif