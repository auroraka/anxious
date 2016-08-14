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

unsigned *sdram = (unsigned *) SDRAM_CONTROLLER_0_BASE;
unsigned *model = (unsigned *) MODEL_MEMORY_BASE;
unsigned *frame = (unsigned *) RECOG_CPU_MEMORY_BASE;
unsigned *row = (unsigned *) RECOG_CPU_MEMORY_BASE + (640 * 480 / 2 / 2 / 8);

#define SDRAM(x, y) sdram[(2 << 19) + ((y) << 10) + (x)]
#define set_frame(x, y, val) ((frame[(y) * 10 + ((x) >> 5)] &= ~(1 << ((x) >> 5))) |= (val) << ((x) >> 5))
#define get_frame(x, y) ((frame[(y) * 10 + ((x) >> 5)] >> ((x) >> 5)) & 1)

#define START_ROW (CPU_ID * 240)
#define ROW_CNT 240

unsigned max(unsigned a, unsigned b) {
	return x ^ ((x ^ y) & -(x < y));
}

unsigned min(unsigned a, unsigned b) {
	return y ^ ((x ^ y) & -(x < y));
}

unsigned H_l = 29, S_l = 86, V_l = 6;
unsigned H_r = 64, S_r = 255, V_r = 255;

unsigned check_range(unsigned pixel) {
	unsigned R, G, B, H, S, V, delta;
	
	R = ((pixel >> 20) & 0x3FF) >> 2;
	G = ((pixel >> 10) & 0x3FF) >> 2;
	B = (pixel & 0x3FF) >> 2;
	V = max(max(R, G), B);
	if (V < V_l || V_r < V) return 0;
	
	delta = V - min(min(R, G), B);
	S = (delta << 8) / V;
	if (S < S_l || S_r < S) return 0;
	
	// coef = 60 * 255 / 360 = 42.5
	if (R == V) H = (G - B) * 42 / delta;
	else if (G == V) H = (B - R) * 42 / delta + 85;
	else H = (R - G) * 42 / delta + 170;
	if (H > 255) H = 255 - H;
	if (H < H_l || H_r < H) return 0;
	return 1;
}

int main() {
	
	#if CPU_ID == 0
	while (1) {
		int i, j;
		unsigned pixel;
		for (j = 0; j < 480; ++j) {
			for (i = 0; i < 640; ++i) {
				pixel = SDRAM(i, j);
				row[i >> 1] += (((pixel >> 16) & 0xFF) << 20)
				               + (((pixel >> 8) & 0xFF) << 10)
				               + (pixel & 0xFF);
			}
			if ((j & 1) == 1) {
				for (i = 0; i < (640 >> 1); ++i)
					set_frame(i, j >> 1, check_range(row[i]));
			}
		}
		printf("HSV & inRange: %f\n", (float) clock() / CLOCKS_PER_SEC);
		
		unsigned *first = row, *last = row + 10, *swp_tmp;
		int delta;
		memset(first, 0xFF, 320);
		for (j = 0; j < 240; ++j) {
			delta = j * 10;
			
			pixel = frame[delta];
			last[0] = pixel & ((pixel >> 1) | ((frame[delta + 1] & 1) << 31)) & ((pixel << 1) | 1);
			for (i = 0; i < 10; ++i) {
				pixel = frame[delta + i];
				last[i] = pixel & ((pixel >> 1) | ((frame[delta + 1] & 1) << 31)) & ((pixel << 1) | (frame[delta - 1] >> 31));
			}
			pixel = frame[delta + 9];
			last[9] = pixel & ((pixel >> 1) | (1 << 31)) & ((pixel << 1) | (frame[8] >> 31));
			
			memcpy(last, frame + j * 10, 320);
			for (i = 0; i < 10; ++i)
				frame[j * 10 + i] &= frame[(j + 1) * 10 + i] & first[i];
			swp_tmp = first;
			first = last;
			last = swp_tmp;
		}
		
		memset(first, 0xFF, 320);
		for (j = 0; j < 240; ++j) {
			delta = j * 10;
			
			pixel = frame[delta];
			last[0] = pixel | ((pixel >> 1) | ((frame[delta + 1] & 1) << 31)) | (pixel << 1);
			for (i = 0; i < 10; ++i) {
				pixel = frame[delta + i];
				last[i] = pixel | ((pixel >> 1) | ((frame[delta + 1] & 1) << 31)) | ((pixel << 1) | (frame[delta - 1] >> 31));
			}
			pixel = frame[delta + 9];
			last[9] = pixel | (pixel >> 1) | ((pixel << 1) | (frame[8] >> 31));
			
			memcpy(last, frame + j * 10, 320);
			for (i = 0; i < 10; ++i)
				frame[j * 10 + i] |= frame[(j + 1) * 10 + i] | first[i];
			swp_tmp = first;
			first = last;
			last = swp_tmp;
		}
		
		printf("Erode & dilate: %f\n", (float) clock() / CLOCKS_PER_SEC);
		
		for (j = 0; j < 480; ++j)
			for (i = 0; i < 640; ++i)
				SDRAM(i, j) = get_frame(i >> 1, j >> 1);
	}
	#endif

	return 0;
}

