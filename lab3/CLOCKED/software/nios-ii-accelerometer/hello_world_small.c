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
    *led = (8 >> val) | (8 << (8 - val));
    *level = (acc_read >> 1) & 0x1f;
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

void timer_init(void *isr) {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0003);
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_BASE, 0);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_BASE, 0x0900);
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_BASE, 0x0000);
    alt_irq_register(TIMER_IRQ, 0, isr);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_BASE, 0x0007);
}

void timer_initialise() {
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER1_BASE, 0x00); // Stop timer during setup
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER1_BASE, 0);    // Clear any pending interrupts
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER1_BASE, 0xFFFF); // Set max period (low)
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER1_BASE, 0xFFFF); // Set max period (high)
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER1_BASE, 0x07); // Start timer in free-running mode
}


alt_u32 timer_read() {
    alt_u16 low = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER1_BASE);
    alt_u16 high = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER1_BASE);
    return ((alt_u32)high << 16) | low;
}


int main() {
//    alt_32 x_read;
//    alt_up_accelerometer_spi_dev *acc_dev;
//    acc_dev = alt_up_accelerometer_spi_open_dev("/dev/accelerometer_spi");
//    if (acc_dev == NULL) {
//        return 1;
//    }
//
//    timer_init(sys_timer_isr);
//    timer_initialise();
//    const float clock_frequency = 50e6; // Adjust based on your clock
//    alt_u32 start_time, end_time, elapsed_cycles;
//    float elapsed_time;
//
//    while (1) {
//        start_time = timer_read();
//
//        // Simulated workload
//        alt_up_accelerometer_spi_read_x_axis(acc_dev, &x_read);
//        convert_read(x_read, &level, &led);
//
//        end_time = timer_read();
//
//        // Handle roll-over
//        elapsed_cycles = (end_time >= start_time) ? (end_time - start_time)
//                                                  : (0xFFFFFFFF - start_time + end_time + 1);
//
//        elapsed_time = elapsed_cycles / clock_frequency;
//
//        printf("Start: %.20f\nEnd: %.20f\n", start_time, end_time);
//        printf("Elapsed time: %.20f seconds\n", elapsed_time);
//    }
	while (1) {
	    alt_u32 time_now = timer_read();
	    printf("Current Timer Value: %lu\n", time_now);
	    alt_u16 period_low = IORD_ALTERA_AVALON_TIMER_PERIODL(TIMER1_BASE);
	    alt_u16 period_high = IORD_ALTERA_AVALON_TIMER_PERIODH(TIMER1_BASE);
	    printf("Timer Period: %u%u\n", period_high, period_low);

	}


    return 0;
}
