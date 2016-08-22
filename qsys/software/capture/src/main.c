/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */
#include <stdio.h>
#include <system.h>
#include <unistd.h>
#include <io.h>
#include <time.h>
 
#include "sccb.h"

#define HEIGHT (480)
#define WIDTH (640)

volatile unsigned *sdram = (unsigned *) SDRAM_CONTROLLER_0_BASE;
#if CPU_ID == 0
volatile unsigned *capture = (unsigned *) CAPTURE_PIO_0_BASE;
unsigned bank = 0;
volatile unsigned *led = (unsigned *) LED_PIO_0_BASE;
#else
volatile unsigned *capture = (unsigned *) CAPTURE_PIO_1_BASE;
unsigned bank = 1;
volatile unsigned *led = (unsigned *) LED_PIO_1_BASE;
#endif

int main() {
	
	int i, j, state = 1;
	configure_sccb();
#if CPU_ID == 0
	for (j = 0; j < HEIGHT; ++j)
		for (i = 0; i < WIDTH; ++i)
			sdram[(1 << 23) | (1 << 19) | (j << 10) | i] = 1 << 24;
#endif
	*led = 3;
	while (1) {
		while (1) {
// 			for (j = 0; j < HEIGHT; ++j)
// #if CPU_ID == 0
// 				for (i = 0; i < WIDTH / 2; ++i)
// 					sdram[(0 << 23) | (1 << 19) | (j << 10) | i] = sdram[(0 << 23) | (j << 10) | i];
// #else
// 				for (i = WIDTH / 2; i < WIDTH; ++i)
// 					sdram[(1 << 23) | (1 << 19) | (j << 10) | i] = ~(1 << 24) & sdram[(1 << 23) | (j << 10) | i];
// #endif
			if (state == 1 && *capture == 0) break;
			state = *capture;
			usleep(1000);
		}
		
		*led = 0;
		printf("[FRAME START]\n");
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
				printf("%u ", sdram[(0 << 23) | (bank << 19) | (j << 10) | i]);
			printf("\n");
		}
		*led = 3;
	}

	return 0;
}
