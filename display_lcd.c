#include <avr/io.h>
#include <util/delay.h>
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y &= (1 << bit_x))
#define ENABLE_PIN PB4
#define REGISTER_SELECT_PIN PB5
#define LCD_PORT PORTB
#define LCD_PORT_MASK 0b00001111
#define PINS_MASK 0b00110000

void write4bits(uint8_t data) {
    uint8_t aux;

    aux = LCD_PORT;
    aux &= ~(LCD_PORT_MASK);
  	data &= LCD_PORT_MASK;
    aux |= data;
    LCD_PORT = aux;
    enable();

    return;
}

void send(uint8_t data, uint8_t kind) {
    if (kind) {
        set_bit(LCD_PORT, REGISTER_SELECT_PIN);
    } else {
        clr_bit(LCD_PORT, REGISTER_SELECT_PIN);
    }
  	uint8_t aux = data >> 4;
    write4bits(data >> 4);
    write4bits(data &= 0b00001111);

}

void send_command(uint8_t cmd) {
    send(cmd, 0);
}

void send_data(uint8_t data) {
    send(data, 1);
}

void enable() {
    clr_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(1);
    set_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(1);
    clr_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(45);

    return;
}

void initialize() {
    _delay_ms(50);
    for (uint8_t i = 0; i < 3; i++) {
        write4bits(0x03);
        _delay_ms(5);
    }
    write4bits(0x02);
    send_command(0x28);
    display_off();
    clear_display();
    display_on();
    send_command(0x06);
}

void clear_display() {
    send_command(0x01);
    _delay_ms(2);
}

void display_on() {
    send_command(0x0C);
}

void display_off() {
    send_command(0x08);
}

void display_num(uint8_t num) {
    send_data(num);
}

void cursor_on() {
    send_command(0x0E);
}

void set_cursor(uint8_t row, uint8_t col) {
    uint8_t address = 0;
    if (row == 1) {
        address = 0x80;
    } else {
        address = 0xC0;
    }
    if (col > 0 && col < 17) {
        address |= (col - 1);
    }
    send_command(address);
}

void display_str(char* message) {
	uint8_t length = 0;
    while (message[length] != '\0') {
        if (message[length] == '\n') {
            set_cursor(2, 1);
        } else {
            send_data(message[length]);
        }
        length++;
    }
}

int main() {
    DDRB |= 0b00111111;
  	PORTB &= 0b11000000;

    initialize();
  	display_str("Leonardo\nVendrame");
    while (1) {
    }

    return 0;
}
