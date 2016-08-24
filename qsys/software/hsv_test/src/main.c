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

#define HEIGHT (480)
#define WIDTH (640)

volatile unsigned *sdram = (volatile unsigned *) NEW_SDRAM_CONTROLLER_0_BASE;
volatile unsigned *hsv_ram = (volatile unsigned *) ONCHIP_MEMORY2_1_BASE;

volatile unsigned *__sdram(unsigned bank, unsigned buffer_port, int x, int y) {
  return &sdram[(bank << 23) | (buffer_port << 19) | (x << 10) | y];
}

volatile unsigned *vga_pixel(int x, int y) {
  return __sdram(1, 0, x, y);
}

void clean_sdram() {
	int i, j, bank;
	for (bank = 0; bank < 3; ++bank) {
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
        *__sdram(1, bank, j, i) = (1 << 24);
		}
		printf("Cleaned bank %d\n", bank);
	}
}

int main() {
	int i, j, k;

	configure_sccb();

  clean_sdram();

  const int kPackLength = 32;
  const int hsv_width = WIDTH / kPackLength;
  const int kHsvFrameSize = WIDTH * HEIGHT / 32;
  const int delay = 0;

  for (;;) {
    // MUST use IOWR instead of writing value through pointer
    IOWR(HSV_RENDER_BUFFER_VSYNC_BASE, 0, 0U);
    unsigned render_buffer_port = IORD(HSV_RENDER_BUFFER_PORT_BASE, 0);

    IOWR(HSV_FETCH_BUFFER_VSYNC_BASE, 0, 0U);
    unsigned fetch_buffer_port = IORD(HSV_FETCH_BUFFER_PORT_BASE, 0);

    volatile unsigned *hsv_frame = hsv_ram + fetch_buffer_port * kHsvFrameSize;

    for (j = 0; j < HEIGHT; ++j) {
      for (i = 0; i < hsv_width; ++i) {
        unsigned hsv = hsv_frame[j * hsv_width + i];
        for (k = 0; k < kPackLength; ++k) {
          *__sdram(1, render_buffer_port, j, i * kPackLength + k) =
            ((hsv >> k) & 1) ? 0xFFFFFF : 0x0;
        }
      }
    }

    IOWR(HSV_FETCH_BUFFER_VSYNC_BASE, 0, 1U);

    IOWR(HSV_RENDER_BUFFER_VSYNC_BASE, 0, 1U);

    // Note that delay can be 0 - no idea why it makes a difference, yet
    // its necessary
    usleep(delay);
  }

	return 0;
}
