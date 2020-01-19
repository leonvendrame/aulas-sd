#include <util/delay.h>
#include <display_lcd.h>
#include <stdio.h>
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))

void enable() {
    clr_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(1);
    set_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(1);
    clr_bit(LCD_PORT, ENABLE_PIN);
    _delay_us(45);
}

void write4bits(uint8_t data) {
    uint8_t aux = LCD_PORT;
    aux &= ~(LCD_PORT_MASK);
    data <<= LCD_LSB_LITTLE_ENDIAN;
    data &= LCD_PORT_MASK;
    aux |= data;
    LCD_PORT = aux;
    enable();
}

void send(uint8_t data, uint8_t kind) {
    if (kind) {
        set_bit(LCD_PORT, REGISTER_SELECT_PIN);
    } else {
        clr_bit(LCD_PORT, REGISTER_SELECT_PIN);
    }
    write4bits(data >> 4);
    write4bits(data & 0x0F);
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

void display_num(uint16_t num) {
    char *str = "65536";
    // if (num < 10) {
    sprintf(str, "%d", num);
    // } else if (num < 100) {
    //     sprintf(str, "%d", num);
    // } else {
    //     sprintf(str, "%d", num);
    // }
    display_str(str);
}

// void clean_half_line_display(uint8_t line, uint8_t half) {
//     if (line < 1 || line > 2) {line = 1};
//     if (half < 1 || half > 2) {half = 1};
//     set_cursor(line, ((half - 1) * 9));
//     display_str("        ");
// }

void initialize_display() {
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

void setup_display() {
    LCD_DDR |= LCD_PORT_MASK;
    set_bit(LCD_DDR, ENABLE_PIN);
    set_bit(LCD_DDR, REGISTER_SELECT_PIN);
}
