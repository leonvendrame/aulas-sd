#include <avr/io.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define mask_port_b 0b00000111
#define mask_port_d 0b11110000

int main() {

    DDRB |= mask_port_b;
    DDRD |= mask_port_d;

  	clr_bit(DDRB, 3);
  	set_bit(PORTB, 3);
  	
    uint8_t numbers[16];
    numbers[0] = 0b00000001;
  	numbers[1] = 0b01001111;
  	numbers[2] = 0b00010010;
  	numbers[3] = 0b00000110;
  	numbers[4] = 0b01001100;
  	numbers[5] = 0b00100100;
  	numbers[6] = 0b00100000;
  	numbers[7] = 0b00001111;
  	numbers[8] = 0b00000000;
  	numbers[9] = 0b00000100;
  	numbers[10] = 0b00001000;
  	numbers[11] = 0b01100000;
  	numbers[12] = 0b00110001;
  	numbers[13] = 0b01000010;
  	numbers[14] = 0b00110000;
  	numbers[15] = 0b00111000;
		
  	uint8_t aux, aux2;
    aux = numbers[0];
  
    PORTB &= ~(mask_port_b);
    aux2 = ((aux>>4) & mask_port_b);
    PORTB |= aux2;
    PORTD &= ~(mask_port_d);
    aux2 = ((aux<<4) & mask_port_d);
    PORTD |= aux2;
  	
  	uint8_t count = 0;
  
    while (1) {
        if (!tst_bit(PINB, 3)) {
          	if (count >= 15) count = 0; else count++;
        	aux = numbers[count];
          	
          	PORTB &= ~(mask_port_b);
            aux2 = ((aux>>4) & mask_port_b);
            PORTB |= aux2;
          	
            PORTD &= ~(mask_port_d);
            aux2 = ((aux<<4) & mask_port_d);
            PORTD |= aux2;

          	while (!tst_bit(PINB, 3)) {}
        }
    }

    return 0;
}
