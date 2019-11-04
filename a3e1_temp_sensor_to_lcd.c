#include <avr/io.h>
#include <util/delay.h>
#include <display_lcd.h>
#include <temp_sensor_dht11.h>

int main() {
    uint8_t temps[5];

    setup_display();
    initialize_display();
    
    while (1) {
        clear_display();
        if (read_temperature(temps) && checksum(temps)) {
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