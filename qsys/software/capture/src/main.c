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
	
	int key_state = 1;
	while (true) {
		
		unsigned center_l = SHARED_R(1), center_r = SHARED_R(0);
		Location loc = find_location(center_l, center_r);
		SHARED_W(2, *(unsigned *)&loc.x);
		SHARED_W(3, *(unsigned *)&loc.y);
		SHARED_W(4, *(unsigned *)&loc.z);
//		printf("%d %d %d\n", (int)loc.x, (int)loc.y, (int)loc.z);
		
		draw_overlay();
		
		if (KEY_R() == 0) {
			if (key_state == 1) {
				key_down(0);
			}
			key_state = 0;
		} else key_state = 1;
	}
#elif CPU_ID > 2
	const int MODULO = HEIGHT % RENDER_CORES;
	int ROW_CNT = HEIGHT / RENDER_CORES + ((CPU_ID - 2 < MODULO) ? 1 : 0);
	int ROW_START = CPU_ID - 2 < MODULO ? ROW_CNT * (CPU_ID - 2) : (ROW_CNT + 1) * MODULO + (CPU_ID - 2 - MODULO) * ROW_CNT;

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
	unsigned render_port = 0;
	while (true) {
		result = recognize();
//		result = recognize_raw(CPU_ID);
		SHARED_W(CPU_ID, result.center);
	}
#endif
	
	return 0;
}
