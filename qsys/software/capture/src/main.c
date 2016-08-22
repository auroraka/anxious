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

#include "common.h"
#include "sccb.h"
#include "recognition.h"
#if CPU_ID == 1
#include "display.h"
#endif

#define HEIGHT (480)
#define WIDTH (640)

volatile unsigned *sdram = (unsigned *) SDRAM_CONTROLLER_0_BASE;

volatile unsigned *shared_memory = (unsigned *) SHARED_MEMORY_BASE;

#if CPU_ID == 0
#define CAPTURE IORD(CAPTURE_PIO_0_BASE, 0)
#define LED(x) IOWR(LED_PIO_0_BASE, 0, x)
unsigned bank = 0;
unsigned *frame = (unsigned *) RECOG_MEMORY_0_BASE;
unsigned *row = (unsigned *) RECOG_MEMORY_0_BASE + (WIDTH * HEIGHT >> 7);
#else
#define CAPTURE IORD(CAPTURE_PIO_1_BASE, 0)
#define LED(x) IOWR(LED_PIO_1_BASE, 0, x)
unsigned bank = 1;
unsigned *frame = (unsigned *) RECOG_MEMORY_1_BASE;
unsigned *row = (unsigned *) RECOG_MEMORY_1_BASE + (WIDTH * HEIGHT >> 7);
#endif

void clean_sdram() {
	int i, j, bank;
	for (bank = 0; bank < 3; ++bank) {
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
				sdram[(1 << 23) | (bank << 19) | ((j) << 10) | (i)] = (1 << 24);
		}
		printf("Cleaned bank %d\n", bank);
	}
}

void capture() {
	int i, j, state = 1;
	LED(1);
	if (state == 1 && CAPTURE == 0) {
		state = CAPTURE;
		LED(0);
		printf("[FRAME START]\n");
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
				printf("%u ", sdram[(0 << 23) | (bank << 19) | (j << 10) | i]);
			printf("\n");
		}
		printf("[FRAME END]\n");
		LED(1);
	} else {
		state = CAPTURE;
	}
}

#if CPU_ID == 1
const float f_l = 1117.368, f_r = 1098.564;
const float c_l = 284.43259, c_r = 260.334;
const float T = 17.6;

char msg[100];
#endif

int main() {
	int i, j;
	
	configure_sccb();

	for (j = 0; j < HEIGHT; ++j)
		for (i = 0; i < WIDTH; ++i)
			sdram[(1 << 23) | (bank << 19) | (j << 10) | i] = 1 << 24;

	RecogResult result, last_result;
	while (true) {
		capture();
		
		last_result = result;
		result = recognize(bank, frame, row);
		clearResult(bank, &last_result);
		drawResult(bank, &result);
		
#if CPU_ID == 0
		shared_memory[0] = result.center;
#else
		unsigned center_0 = shared_memory[0];
		int x_l = get_x(result.center), x_r = get_x(center_0);
		float d = T / ((x_r - c_r) / f_r + (c_l - x_l) / f_l);
		clearMsg(0, msg, 20, 20);
		clearMsg(1, msg, 20, 20);
		// printf("%.1lf\n", d);
		sprintf(msg, "Left %dx%d\nRight %dx%d\nDistance %d",
			get_x(result.center), get_y(result.center),
			get_x(center_0), get_y(center_0), (int)d);
		drawMsg(0, msg, 20, 20);
		drawMsg(1, msg, 20, 20);
#endif
	}

	return 0;
}
