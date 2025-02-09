#include <sys/alt_stdio.h>
#include <stdio.h>
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include <unistd.h>

#define DELAY 20000 // 50ms

int main()
{
    int switch_datain = 0;
    int led_pattern[14] = {
        0b0000000001,
        0b0000000011,
        0b0000000111,
        0b0000001111,
        0b0000011111,
        0b0000111110,
        0b0001111100,
        0b0011111000,
        0b0111110000,
        0b1111100000,
        0b1111000000,
        0b1110000000,
        0b1100000000,
        0b1000000000
    };
    int pattern_index = 0;

    // Circling pattern across 6 7-segment displays
    int circling_pattern = 0b0110110;
    int circle_index = 0;

    while (1)
    {
        switch_datain = ~IORD_ALTERA_AVALON_PIO_DATA(BUTTON_BASE);
        switch_datain &= (0b0000000011);

        if (switch_datain & 0b01) {
            // LED pattern functionality remains the same
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, led_pattern[pattern_index]);
            pattern_index = (pattern_index + 1) % 14;
            usleep(DELAY);
        }
        else if (switch_datain & 0b10) {
            // Circling pattern: one segment "travels" across adjacent displays
            IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, (circle_index == 0) ? circling_pattern : 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, (circle_index == 1) ? circling_pattern : 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, (circle_index == 2) ? circling_pattern : 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, (circle_index == 3) ? circling_pattern : 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, (circle_index == 4) ? circling_pattern : 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, (circle_index == 5) ? circling_pattern : 0b1111111);

            circle_index = (circle_index + 1) % 6; // Move the active segment
            usleep(DELAY * 3);
        }
        else {
            // Turn off all LEDs and 7-segment displays
            IOWR_ALTERA_AVALON_PIO_DATA(LED_BASE, 0b0000000000);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX0_BASE, 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX1_BASE, 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX2_BASE, 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX3_BASE, 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX4_BASE, 0b1111111);
            IOWR_ALTERA_AVALON_PIO_DATA(HEX5_BASE, 0b1111111);
        }
    }

    return 0;
}
