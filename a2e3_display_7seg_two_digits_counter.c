#include <avr/io.h>
#include <avr/pgmspace.h>
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

void display_two_digits(uint8_t digits[]) {
    display_single_digit(digits[0]);
    _delay_ms(300);
    clear_display();
    _delay_ms(200);
    display_single_digit(digits[1]);
    _delay_ms(300);
    clear_display();

    return;
}

void clear_display() {
    PORTB |= 0b00000111;
    PORTD |= 0b11110000;

    return;
}

void prepare_digits(uint8_t count, uint8_t digits[], const uint8_t display_map[]) {
    digits[0] = pgm_read_byte(&display_map[count >> 4]);
    digits[1] = pgm_read_byte(&display_map[count & 0b00001111]);

    return;
}

int main() {
    uint8_t count = 0, digits[2];
    const PROGMEM static uint8_t display_map[16] = {
        0b00000001, 0b01001111,
        0b00010010, 0b00000110,
        0b01001100, 0b00100100,
        0b00100000, 0b00001111,
        0b00000000, 0b00000100,
        0b00001000, 0b01100000,
        0b00110001, 0b01000010,
        0b00110000, 0b00111000
    };

    DDRB |= mask_port_b;
    DDRD |= mask_port_d;

    DDRB &= 0b11100111;
    PORTB |= 0b00011000;

    prepare_digits(count, digits, display_map);
    clear_display();
    _delay_ms(300);
    display_two_digits(digits);
  
    while (1) {
        if (!tst_bit(PINB, 3)) {
            _delay_ms(12);
            count++;

            prepare_digits(count, digits, display_map);
            display_two_digits(digits);

            while (!tst_bit(PINB, 3)) {}
            _delay_ms(24);
        }

        if (!tst_bit(PINB, 4)) {
            _delay_ms(12);
            count--;

            prepare_digits(count, digits, display_map);
            display_two_digits(digits);

            while (!tst_bit(PINB, 4)) {}
            _delay_ms(24);
        }
    }

    return 0;
}
