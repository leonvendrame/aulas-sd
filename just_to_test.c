#include <util/delay.h>
#include <stdio.h>
#include <display_lcd.h>
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) ((Y &= (1 << bit_x)) >> bit_x)
#define ENABLE_PIN PB4
#define REGISTER_SELECT_PIN PB5
#define LCD_PORT PORTB
#define LCD_PORT_MASK 0b00001111

#define HIGH 1
#define LOW 0
#define SENSOR_IO_PIN PD2
#define SENSOR_PIN PIND
#define SENSOR_PORT PORTD
#define SENSOR_DDR DDRD

void enable() {
    clr_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(1);
    set_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(1);
    clr_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(45);

    return;
}

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
    write4bits(data >> 4);
    write4bits(data &= 0b00001111);

}

void send_command(uint8_t cmd) {
    send(cmd, 0);
}

void send_data(uint8_t data) {
    send(data, 1);
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

void display_num(uint8_t num) {
  	char *str = "000";
  	sprintf(str, "%d", num);
    display_str(str);
}

uint16_t expect_pulse(uint8_t level) {
	uint16_t count = 0;
	while (tst_bit(SENSOR_PIN, SENSOR_IO_PIN) != level && count < 16000) {
		count++;
	}
	return count;
}

uint8_t checksum(uint8_t temps[]) {
    uint8_t sum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        sum += temps[i];
    }
    if (temps[4] == sum) {
        return 1;
    }
    return 0;
}

uint8_t read_temperature(uint8_t read_temp[5]) {
	uint8_t count = 0, bitwise_count = 0;
	uint16_t time_count[2] = {0, 0};

	set_bit(SENSOR_DDR, SENSOR_IO_PIN);

	set_bit(SENSOR_PORT, SENSOR_IO_PIN);
	_delay_us(1);

	clr_bit(SENSOR_PORT, SENSOR_IO_PIN);
	_delay_ms(18);

	set_bit(SENSOR_PORT, SENSOR_IO_PIN);
	clr_bit(SENSOR_DDR, SENSOR_IO_PIN);
	set_bit(SENSOR_PORT, SENSOR_IO_PIN);
    if (expect_pulse(LOW) >= 16000) {
        return 1;
    }

    if (expect_pulse(HIGH) >= 16000) {
        return 1;
    }
    if (expect_pulse(LOW) >= 16000) {
        return 1;
    }

	while (count < 5) {
        while (bitwise_count < 8) {
            read_temp[count] <<= 1;
            time_count[0] = expect_pulse(HIGH);
            time_count[1] = expect_pulse(LOW);
            if (time_count[0] >= 16000 || time_count[1] >= 16000) { 
                return 1; 
            }
            if (time_count[0] > time_count[1]) {
	    		read_temp[count] |= 0;
			} else {
                read_temp[count] |= 1;
		    }
            bitwise_count++;
        }
        bitwise_count = 0;
        count++;
	}
    return 0;
}

int main() {
    DDRB |= 0b00111111;
  	PORTB &= 0b11000000;
    uint8_t temps[5];
    initialize();
    
    while (1) {
        clear_display();
        if (!read_temperature(temps) && checksum(temps)) {
                display_str("Temp.: ");
                display_num(temps[2]);
                display_str(" C\nUmid.: ");
                display_num(temps[0]);
                display_str("%");
        } else {
            display_str("Read Error");
        }
        _delay_ms(3000);
    }
    return 0;
}
