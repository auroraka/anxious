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

//#define CPU_ID 2

#if CPU_ID == 2
	#include "control.h"
	#include "render/object.h"
#elif CPU_ID > 2
	#include "render.h"
#else
	#include "sccb.h"
	#include "recognition.h"
#endif

int main() {
	
	printf("CPU: %d\n", CPU_ID);

#if CPU_ID == 2
	// Controller
	clean_sdram(1);
	clean_sdram(2);
	reset_objects();
	
	unsigned key_state = 1, key_code = 0xFF;
	while (true) {
		
		unsigned center_l = SHARED_R(0), center_r = SHARED_R(1);
		Location loc = find_location(center_l, center_r);
		SHARED_W(2, *(unsigned *)&loc.x);
		SHARED_W(3, *(unsigned *)&loc.y);
		SHARED_W(4, *(unsigned *)&loc.z);
//		printf("%d %d %d\n", (int)loc.x, (int)loc.y, (int)loc.z);
		
		draw_overlay();
		
		key_code = IR_R();
		if (key_code != 0xFF) {
			if (key_state == 1) {
				key_down(key_code);
			}
			key_state = 0;
		} else key_state = 1;
	}
#elif CPU_ID > 2
	const int MODULO = HEIGHT % RENDER_CORES;
	int ROW_CNT = HEIGHT / RENDER_CORES + ((CPU_ID - 3 < MODULO) ? 1 : 0);
	int ROW_START = CPU_ID - 3 < MODULO ? ROW_CNT * (CPU_ID - 3) : (ROW_CNT + 1) * MODULO + (CPU_ID - 3 - MODULO) * ROW_CNT;

	// Renderer
	render_init(ROW_START, ROW_CNT);
	while (true) {
		render(ROW_START, ROW_CNT);
	}
#else
	// Camera recognition
	configure_sccb();
	printf("CPU: %d\n", CPU_ID);
	
	RecogResult result;
	int i, j, cnt;
//	unsigned render_port = 0;
//	const unsigned MASK_WIDTH = WIDTH >> 5;
//	volatile unsigned *frame = (unsigned *)0x08000000 + (HEIGHT * MASK_WIDTH);
	while (true) {
//		result = recognize();
		result = recognize_raw(CPU_ID);
//#define get_frame(x, y) ((frame[(y) * MASK_WIDTH + ((x) >> 5)] >> ((x) & 31)) & 1)
		SHARED_W(CPU_ID, result.center);
		
//		for (j = 0; j < 480; ++j)
//#if CPU_ID == 0
//			for (i = 0; i < 320; ++i) {
//#else
//			for (i = 320; i < 640; ++i) {
//#endif
//				if (get_frame(i, j)) SDRAM_W(i, j, WHITE);
//				else SDRAM_W(i, j, 0);
//			}
	}
#endif
	
	return 0;
}
