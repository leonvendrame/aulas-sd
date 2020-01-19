#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include<display_lcd.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x))
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))

#define DISTANCE_SENSOR_PORT PORTB
#define DISTANCE_SENSOR_PIN PINB
#define DISTANCE_SENSOR_DDR DDRB
#define DISTANCE_SENSOR_TRIGGER PB1
#define DISTANCE_SENSOR_ECHO PB0
// #define SOUND_SPEED_CM_US 58
#define COUNTER_DIVISOR_TO_US 118

volatile uint8_t ignore_next_read_flag = 0;

void setup_distance_sensor() {
    DISTANCE_SENSOR_DDR |= 1 << DISTANCE_SENSOR_TRIGGER;
    DISTANCE_SENSOR_DDR &= ~(1 << DISTANCE_SENSOR_ECHO);
    DISTANCE_SENSOR_PORT &= ~((1 << DISTANCE_SENSOR_TRIGGER) | (1 << DISTANCE_SENSOR_ECHO));
}

void distance_sensor_enable_trigger() {
    set_bit(DISTANCE_SENSOR_PORT, DISTANCE_SENSOR_TRIGGER);
    _delay_us(10);
    clr_bit(DISTANCE_SENSOR_PORT, DISTANCE_SENSOR_TRIGGER);
}

uint16_t distance_in_cm() {
    distance_sensor_enable_trigger();
    
    while (!tst_bit(DISTANCE_SENSOR_PIN, DISTANCE_SENSOR_ECHO));
    TCNT1 = 0;
    while (tst_bit(DISTANCE_SENSOR_PIN, DISTANCE_SENSOR_ECHO) && TCNT1 < 48000);
    
    return TCNT1 / COUNTER_DIVISOR_TO_US;
}

void setup_timer_counter_1() {
    TCCR1B = 1 << CS11;
}

int main() {
    setup_display();
    setup_distance_sensor();
    setup_timer_counter_1();
    initialize_display();
    
    volatile uint16_t distance = 0;
    display_str("Dist.:");

    while (1) {
        distance = distance_in_cm();
        set_cursor(1, 8);
        display_str("        ");
        set_cursor(1, 8);
        if (distance >= 5 && distance <= 400) {
            display_num(distance);
            display_str(" cm");
        } else if (distance < 5) { 
            display_str("<5 cm");
        } else {
            display_str(">400 cm");
        }
        _delay_ms(1000);
    }

    return 0;
}