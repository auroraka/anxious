/*
 * CPU Assignments:
 *   0, 1 : recognition cores for cameras 0 & 1
 *   2    : controller core
 *   > 2  : render cores
 */
#include <stdio.h>
#include <unistd.h>
#include <io.h>
#include <time.h>

#include "common.h"
#include "memory.h"

#define CPU_ID 2

#if CPU_ID == 2
	#include "control.h"
#elif CPU_ID > 2
	#include "render.h"
#else
	#include "sccb.h"
	#include "recognition.h"
	#include "control.h"
#endif

#if CPU_ID < 2
//#define CAPTURE IORD(CAPTURE_PIO_0_BASE, 0)
//#define LED(x) IOWR(LED_PIO_0_BASE, 0, x)
unsigned port = CPU_ID;
unsigned *frame = (unsigned *)RECOG_MEMORY_0_BASE;
unsigned *row = (unsigned *)RECOG_MEMORY_0_BASE + (WIDTH * HEIGHT >> 7);

void capture() {
	int i, j, state = 1;
	LED(1);
	if (state == 1 && CAPTURE == 0) {
		state = CAPTURE;
		LED(0);
		printf("[FRAME START]\n");
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
				printf("%u ", IORD(SDRAM, (0 << 23) | (port << 19) | (j << 10) | i));
			printf("\n");
		}
		printf("[FRAME END]\n");
		LED(1);
	} else {
		state = CAPTURE;
	}
}
#endif

#if CPU_ID > 2
const int MODULO = HEIGHT % RENDER_CORES;
int ROW_CNT = HEIGHT / RENDER_CORES + ((CPU_ID - 2 < MODULO) ? 1 : 0);
int ROW_START = CPU_ID - 2 < MODULO ? ROW_CNT * (CPU_ID - 2) : (ROW_CNT + 1) * MODULO + (CPU_ID - 2 - MODULO) * ROW_CNT;
#endif

int main() {

#if CPU_ID == 2
	// Controller
	clean_render();
	
	int key_state = 1;
	while (true) {
		unsigned center_l = SHARED_R(1), center_r = SHARED_R(0);
		Location loc = find_location(center_l, center_r, true);
		SHARED_W(2, *(unsigned *)&loc.x);
		SHARED_W(3, *(unsigned *)&loc.y);
		SHARED_W(4, *(unsigned *)&loc.z);
		
		draw_overlay();
		
		if (KEY_R() == 0) {
			if (key_state == 1) {
				key_down(0);
			}
			key_state = 0;
		} else key_state = 1;
	}
#elif CPU_ID > 2
	// Renderer
	render_init(ROW_START, ROW_CNT);
	while (true) {
		render(ROW_START, ROW_CNT);
	}
#else
	// Camera recognition
	configure_sccb();
	
	RecogResult result, last_result;
	while (true) {
//		capture();
		
		last_result = result;
//		result = recognize();
		result = recognize_raw(port, frame, row);
		clear_result(port, &last_result);
		draw_result(port, &result);
		
		SHARED_W(port, result.center);
	}
#endif
	
	return 0;
}
