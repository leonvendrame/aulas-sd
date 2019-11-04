#include <avr/io.h>

#define ENABLE_PIN PB4
#define REGISTER_SELECT_PIN PB5
#define LCD_PORT PORTB
#define LCD_DDR DDRB
#define LCD_PORT_MASK 0b00001111

void enable();

void write4bits(uint8_t data);

void send(uint8_t data, uint8_t kind);

void send_command(uint8_t cmd);

void send_data(uint8_t data);

void clear_display();

void display_on();

void display_off();

void set_cursor(uint8_t row, uint8_t col);

void display_str(char* message);

void display_num(uint8_t num);

void initialize_display();

void setup_display();