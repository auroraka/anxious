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

#if CPU_ID == 2
	#include "control.h"
	#include "render/object.h"
#elif CPU_ID > 2
	#include "render/render.h"
#else
	#include "sccb.h"
	#include "recognition.h"
#endif


int main() {

#if CPU_ID == 2

	// Controller
	reset_objects();
	clean_sdram(1);
	clean_sdram(2);
	clear_ssram();
	controller_init();
	
	unsigned key_state = 1, key_code = 0xFF;
	while (true) {
		
		unsigned center_l = SHARED_R(1), center_r = SHARED_R(0);
		pointf loc = find_location(center_l, center_r);
		SHARED_W(2, *(unsigned *)&loc.x);
		SHARED_W(3, *(unsigned *)&loc.y);
		SHARED_W(4, *(unsigned *)&loc.z);
		
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
	
	RecogResult result;
	int i, j, cnt;
	while (true) {
		result = recognize_raw(CPU_ID);
		result.center = point(WIDTH - get_x(result.center) - 1, get_y(result.center));
		SHARED_W(CPU_ID, result.center);
	}
#endif

	
	return 0;
}
