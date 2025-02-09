#include "system.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include <stdlib.h>
#include <stdio.h>

#define OFFSET -32
#define PWM_PERIOD 16

alt_8 pwm = 0;
alt_u8 led;
int level;

void led_write(alt_u8 led_pattern) {
    IOWR(LED_BASE, 0, led_pattern);
}

void convert_read(alt_32 acc_read, int * level, alt_u8 * led) {
    acc_read += OFFSET;
    alt_u8 val = (acc_read >> 6) & 0x07;
    * led = (8 >> val) | (8 << (8 - val));
    * level = (acc_read >> 1) & 0x1f;
}

void sys_timer_isr() {
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);

    if (pwm < abs(level)) {

        if (level < 0) {
            led_write(led << 1);
        } else {
            led_write(led >> 1);
        }

    } else {
        led_write(led);
    }

    if (pwm > PWM_PERIOD) {
        pwm = 0;
    } else {
        pwm++;
    }

}

void timer_init(void * isr) {

    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);

}

int main() {
    alt_32 x_read;
    alt_up_accelerometer_spi_dev *acc_dev;

    // Open accelerometer device
    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
    if (acc_dev == NULL) {
        printf("Error: Could not open accelerometer device\n");
        return 1;
    }

    // Define clock frequency (in Hz)
    const float clock_frequency = 50e6; // Example: 50 MHz

    while (1) {
        // Read the timer before starting the operation
        alt_u32 start_time = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_BASE) |
                             (IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_BASE) << 16);

        // Perform accelerometer reading
        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);

        // Simulate a small delay for debugging (can remove later)
        for (volatile int i = 0; i < 10000; i++);

        // Read the timer after completing the operation
        alt_u32 end_time = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_BASE) |
                           (IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_BASE) << 16);

        // Handle timer wraparound (optional)
        alt_u32 elapsed_cycles;
        if (end_time < start_time) {
            elapsed_cycles = (0xFFFFFFFF - start_time) + end_time + 1;
        } else {
            elapsed_cycles = end_time - start_time;
        }

        // Convert cycles to elapsed time in seconds
        float elapsed_time = elapsed_cycles / clock_frequency;

        // Print the result
//        printf("Elapsed time for accelerometer read: %.6f seconds\n", elapsed_time);
        printf("Start: %u, End: %u, Elapsed cycles: %u\n", start_time, end_time, elapsed_cycles);

    }

    return 0;
}

