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
	#include "render/render.h"
#else
	#include "sccb.h"
	#include "recognition.h"
#endif

int getRand(){
	return rand();
}
int getCheckNum(){
	int j;
	int x=getRand();
	if (x&1) x^=1;
	if ((x>>16)&1) x^=(1<<16);
	for (j=31;j>16;j--) if ((x>>j)&1) x^=(1<<16);
	for (j=15;j>0;j--) if ((x>>j)&1) x^=1;
	return x;
}
int checkNum(int x){
	int y=0,z=0,i;
	for (i=31;i>16;i--) if ((x>>i)&1) z^=1;
	for (i=15;i>0;i--) if ((x>>i)&1) y^=1;
	return (y==(x&1)) && (z==((x>>16)&1));
}

int main() {
	
	//printf("CPU: %d\n", CPU_ID);

#if CPU_ID == 2
	// int i;
	// for (i=5;i<10;i++){
		// IntF x;
		// x.f=(float)(i+0.123);
		// SHARED_W(i,x.u);	
		// printf("write %d: i=%d f=%d | %d\n",x.u,(int)x.f,(int)x.u);
	// }
	// return 0;
	
	// int i;
	// getCheckNum();
	// printf("cpu2\n");
	// for (i=0;i<30;i++){
		// int x=getCheckNum();
		// printf("write num[%d]: %d\n",i,x);
		// SHARED_W(i,x);
	// }
	//return 0;

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
	// int i;
	// for (i=5;i<10;i++){
		 // IntF x;
		 // x.u=SHARED_R(i);
		 // printf("check %d: i=%d f=%d [%d]\n",i,x.u,(int)x.f,x.f==(float)(i+0.123));
		// //unsigned x=SHARED_R(i);
	// //	printf("check %d: i=%d\n",i,x);
	// }
	// return 0;
	
	// printf("cpu3\n");
	// int i;
	// for (i=0;i<30;i++){
		// // /printf("read...\n");
		// int x=SHARED_R(i);
		// if (checkNum(x)==0){
			// printf("check faild %d: %d[%d]\n",i,x,checkNum(x));
		// }
		// //printf("check num[%d]: %d [%d]\n",i,x,checkNum(x));
	// }
	//return 0;
	

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
	//printf("CPU: %d\n", CPU_ID);
	
	RecogResult result;
	int i, j, cnt;
	while (true) {
//		result = recognize();
		result = recognize_raw(CPU_ID);
		result.center = point(WIDTH - get_x(result.center) - 1, get_y(result.center));
//		printf("%d %d\n", get_x(result.center), get_y(result.center));
		SHARED_W(CPU_ID, result.center);
	}
#endif

	
	return 0;
}
