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

void set_sram(int x, int y, unsigned col) {
  int cnt = x * WIDTH + y;
  int offset = cnt / 8;
  int bit = cnt % 8;
  unsigned val = IORD(sram, offset);
  val = val & (~(0xF << (bit * 4))) | (col << (bit * 4));
  IOWR(sram, offset, val);
}

int main() {
  int col, row;
  int shift = 0;
  for (;;) {
    for (row = 0; row < HEIGHT; ++row) {
      for (col = 0; col < WIDTH; ++col) {
        set_sram(row, col, (col - shift) / 40);
      }
    }
    shift = (shift + 1) % 640;
  }
  return 0;
}
