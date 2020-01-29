#define BAUD 9600

#include<avr/io.h>
#include<avr/sfr_defs.h>
#include<util/setbaud.h>
#include<uart.h>
#include<stdio.h>
#include<string.h>

#define TYPE_LED "LED"
#define TYPE_PIN "PIN"

#define OP_ON "ON"
#define OP_OFF "OFF"
#define OP_STATUS "?"
#define OP_INPUT "INPUT"
#define OP_INPUT_PULLUP "INPUT_PULLUP"
#define OP_OUTPUT "OUTPUT"
#define OP_HIGH "HIGH"
#define OP_LOW "LOW"

#define LEDS_PORT PORTB
#define LEDS_PIN PINB
#define LEDS_DDR DDRB

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))

const char LEDS_IO[] = {PB0, PB1, PB2, PB3};

const char PINS_NUMBER[] = {
    PD0, PD1, PD2, PD3, PD4, 
    PD5, PD6, PD7, PB0, PB1,
    PB2, PB3, PB4, PB5
};

volatile uint8_t *pin_to_ddr(uint8_t pin_number) {
    if (pin_number < 8) {
        return &DDRD;
    } else {
        return &DDRB;
    }
}

volatile uint8_t *pin_to_port(uint8_t pin_number) {
    if (pin_number < 8) {
        return &PORTD;
    } else {
        return &PORTB;
    }
}

void op_led(uint8_t target, char *operation) {
    if (target < 1 || target > 4) {
        putstring("LED TARGET ERROR\n");
        return;
    }
    if (!strcmp(operation, OP_ON)) {
        LEDS_PORT |= 1<<LEDS_IO[target-1];
    } else if (!strcmp(operation, OP_OFF)) {
        LEDS_PORT &= ~(1<<LEDS_IO[target-1]);
    } else if (!strcmp(operation, OP_STATUS)) {
        char led_status[16];
        if (tst_bit(LEDS_PORT, LEDS_IO[target-1])) {
            sprintf(led_status, "LED:%d=ON\n", target);
        } else {
            sprintf(led_status, "LED:%d=OFF\n", target);
        }
    putstring(led_status);
    }
}

void op_pin(uint8_t target, char *operation) {
    if (target > 13) {
        putstring("PIN TARGET ERROR\n");
        return;
    }
    if (!strcmp(operation, OP_INPUT)) {
        clr_bit(*pin_to_ddr(target), PINS_NUMBER[target]);
    } else if (!strcmp(operation, OP_INPUT_PULLUP)) {
        clr_bit(*pin_to_ddr(target), PINS_NUMBER[target]);
        set_bit(*pin_to_port(target), PINS_NUMBER[target]);
    } else if (!strcmp(operation, OP_OUTPUT)) {
        set_bit(*pin_to_ddr(target), PINS_NUMBER[target]);
    } else if (!strcmp(operation, OP_HIGH)) {
        set_bit(*pin_to_port(target), PINS_NUMBER[target]);
    } else if (!strcmp(operation, OP_LOW)) {
        clr_bit(*pin_to_port(target), PINS_NUMBER[target]);
    } else if (!strcmp(operation, OP_STATUS)) {
        char pin_status[21];
        if (tst_bit(*pin_to_ddr(target), PINS_NUMBER[target])) {
            if (tst_bit(*pin_to_port(target), PINS_NUMBER[target])) {
                sprintf(pin_status, "PIN:%d=OUTPUT;HIGH\n", target);
            } else {
                sprintf(pin_status, "PIN:%d=OUTPUT;LOW\n", target);
            }
        } else {
            if (tst_bit(*pin_to_port(target), PINS_NUMBER[target])) {
                sprintf(pin_status, "PIN:%d=INPUT_PULLUP\n", target);
            } else {
                sprintf(pin_status, "PIN:%d=INPUT\n", target); 
            }
        }
        putstring(pin_status);
    }
}

void operation_handler(char *type, uint8_t target, char *operation) {
    if (!strcmp(type, TYPE_LED)) {
        op_led(target, operation);
    } else if (!strcmp(type, TYPE_PIN)) {
        op_pin(target, operation);
    } else {
        putstring("OPERATION ERROR\n");
    }
}

void leds_init() {
    LEDS_DDR |= (1<<LEDS_IO[0] | 1<<LEDS_IO[1] | 1<<LEDS_IO[2] | 1<<LEDS_IO[3]);
}

int main() {
    char str_read[50];
    char type[4], operation[13];
    uint8_t target;

    uart_init();
    leds_init();
  
    while(1) {
        getstring(str_read);
        sscanf(str_read, "%3s:%i=%s", type, &target, operation);
        operation_handler(type, target, operation);
    }

    return 0;
}
