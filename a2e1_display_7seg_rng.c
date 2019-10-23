#include <avr/io.h>
#include <util/delay.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define mask_port_b 0b00000111
#define mask_port_d 0b11110000

void display_single_digit(uint8_t digit) {
	uint8_t aux;
          	
    PORTB &= ~(mask_port_b);
    aux = ((digit >> 4) & mask_port_b);
    PORTB |= aux;

    PORTD &= ~(mask_port_d);
    aux = ((digit << 4) & mask_port_d);
    PORTD |= aux;

	return;
}

int main() {
	uint8_t count = 0;
	uint8_t display_map[10] = {
		0b00000001,	0b01001111,
		0b00010010,	0b00000110,
		0b01001100,	0b00100100,
		0b00100000,	0b00001111,
		0b00000000,	0b00000100
	};

    DDRB |= mask_port_b;
    DDRD |= mask_port_d;

  	clr_bit(DDRB, 3);
  	set_bit(PORTB, 3);
  	
	display_single_digit(0b01111111);
  
    while (1) {
		while (tst_bit(PINB, 3)) {
			count++;
		}
		if (!tst_bit(PINB, 3)) {
			_delay_ms(12);
			count = count % 10;

			display_single_digit(display_map[count]);

			while (!tst_bit(PINB, 3)) {}
			_delay_ms(24);
		}
    }

    return 0;
}
