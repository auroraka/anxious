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

#define DATA(i) (((unsigned)(i)) & 0xFFFFFFFF)
// #define DATA(i) ((0x0))

volatile unsigned* sram = (volatile unsigned *)SSRAM_MM_0_GENERIC_TRISTATE_CONTROLLER_0_BASE;

int main() {
  printf("SSRAM Test.\n");
  unsigned val = 0xFFFF;
  int i;
  // int n = 0x20018; // ptr
  // int n = 0x20000; // IOWR & IORD - max
  int n = 0x80000;
  for (i = 0; i < n; ++i) {
    // printf("write\n");
    IOWR(sram, i, DATA(i));
    // sram[i] = DATA(i);
    if ((i & (0xFFF)) == 0) {
      printf("Written %d\n", i);
    }
  }
  int fail = 0;
  int cnt_cir = 0;
  // for (i = 0; i < n; ++i) {
  for (i = n - 1; i >= 0; --i) {
    if ((i & (0xFFF)) == 0) {
      printf("%d\n", i);
    }
    unsigned act = IORD(sram, i);
    // unsigned act = sram[i];
    unsigned expected = DATA(i);
    if (act != expected) {
      if ((act - expected) & 0x1FFFF) {
        printf("at %d, act = %u, expected = %u\n", i, act, expected);
        fail = 1;
        break;
      } else {
        cnt_cir++;
      }
    }
  }
  printf("cnt_cir = %d\n", cnt_cir);
  if (!fail) printf("Passed all tests: %d\n", n);
	return 0;
}
