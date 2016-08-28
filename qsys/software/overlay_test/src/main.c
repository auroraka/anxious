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

#define HEIGHT (480)
#define WIDTH (640)
#define DATA(i) (((unsigned)(i)) & 0xFFFFFFFF)

volatile unsigned* sram = (volatile unsigned *)SSRAM_MM_0_GENERIC_TRISTATE_CONTROLLER_0_BASE;

volatile unsigned* overlay_buffer_port = (volatile unsigned *)OVERLAY_BUFFER_PORT_BASE;

volatile unsigned* overlay_buffer_vsync = (volatile unsigned *)OVERLAY_BUFFER_VSYNC_BASE;

void set_sram(int buffer_port, int x, int y, unsigned col) {
  int cnt = buffer_port * HEIGHT * WIDTH + x * WIDTH + y;
  int offset = cnt / 8;
  int bit = cnt % 8;
  unsigned val = IORD(sram, offset);
  val = val & (~(0xF << (bit * 4))) | (col << (bit * 4));
  IOWR(sram, offset, val);
}

int main() {
  int col, row;
  int shift = 0;
  printf("Starting...\n");
  for (col = 0; col < (1 << 19); ++col) {
    IOWR(sram, col, 0x33333333);
  }
  printf("Cleaned.\n");
  usleep(1000* 1000* 3);
  for (;;) {
    IOWR(overlay_buffer_vsync, 0, 0);
    int buffer_port = IORD(overlay_buffer_port, 0);
    printf("buffer_port = %d\n", buffer_port);
    // int buffer_port = 0;
    for (row = 0; row < HEIGHT; ++row) {
      for (col = 0; col < WIDTH; ++col) {
        set_sram(buffer_port, row, col, (col + 640 - shift) % 640 / 40);
      }
    }
    IOWR(overlay_buffer_vsync, 0, 1);
    usleep(0);
    // break;
    shift = (shift + 1) % 640;
  }
  return 0;
}
