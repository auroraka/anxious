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

unsigned *sdram = (unsigned *)SDRAM_CONTROLLER_0_BASE;

#ifndef START_ROW
#define START_ROW 0
#endif

#ifndef ROW_CNT
#define ROW_CNT 100
#endif

int main() {
	/* printf("SDRAM Memory Test.\n"); */
	/* int i, n = 10; */
	/* for (i = 0; i < n; ++i) { */
		/* sdram[i] = (unsigned)i; */
	/* } */
	/* for (i = 0; i < n; ++i) { */
		/* printf("sdram[%d] = %u\n", i, sdram[i]); */
	/* } */

/*	printf("Camera.\n");
	usleep(1000 * 1000);
	int i, j, cnt = 0, error = 0, bank;
	unsigned d, r, g, b;
	for (bank = 0; bank < 3; ++bank) {
		cnt = 0;
		error = 0;
		for (i = 0; i < 480; ++i) {
			for (j = 0; j < 640; ++j) {
				d = sdram[(bank << 19) + (i << 10) + j];
				r = d & 0xFF;
				g = (d & 0xFF00) >> 8;
				b = (d & 0xFF0000) >> 16;
				if (r != g || r != b || g != b) {
		//				printf("Check error at (%d, %d)\n", i, j);
					++error;
				}
				if (r == 0 && g == 0 && b == 0)
					++cnt;
			}
		}
		printf("Bank %d:\n", bank);
		printf("\tBlack count: %d\n", cnt);
		printf("\tError count: %d\n", error);
	}*/
	
	int i, j;
	unsigned r, g, b;
	for (i = START_ROW; i < START_ROW + ROW_CNT; ++i)
		for (j = 0; j < 640; ++j) {
			r = j * (255.0 / 640);
			g = r;
			b = r;
			sdram[(2 << 19) + (i << 10) + j] = (r << 16) + (g << 8) + b;
		}

	return 0;
}

