
#ifndef CPU_ID
#define CPU_ID 3
#endif

#if CPU_ID > 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

#include "render.h"
#include "memory.h"
#include "common.h"
#include "palette.h"

#include "render/basic.h"
#include "render/vector.h"
#include "render/object.h"

#include "render/renderer.h"

void render_init(int row_start, int row_cnt) {
	initBuffer();
}

inline void debugVector(Vector A){
	printf("%d %d %d\n",(int)A[0],(int)A[1],(int)A[2]);
}
static int OBJTot=0;
void sync_objects() {
	int i;
	RawSphere sphere;
	int cnt = OBJECT_CNT_R();
	//printf("\ncnt:%d\n",cnt);
	if (cnt<OBJTot) OBJTot=0;
	for (i = OBJTot; i <cnt; ++i) {
		pointf p=raw_objects[i].p[0];
		//printf("get: %d %d %d\n",(int)p.x,(int)p.y,(int)p.z);
		if (p.z==23333){//sphere
			int x=PIC_W-raw_objects[i].p[0].x;
			int y=PIC_H-raw_objects[i].p[0].y;
			int r=raw_objects[i].p[1].x;
			unsigned c=raw_objects[i].color;
			printf("sphere:[id:%d] %d %d %d\n",i,x,y,r);
			Pos2 p={x,y};
			Color color={((palette_colors[c]>>16)&255)/255.0,((palette_colors[c]>>8)&255)/255.0,(palette_colors[c]&255)/255.0};
			if (x-r<0 || x+r>=PIC_W || y-r<0 || y+r>=PIC_H) continue;
			drawSphereC(p,r,color);
		}else{
			pointf o=raw_objects[i].p[0];
			pointf x=raw_objects[i].p[1];
			pointf y=raw_objects[i].p[2];
			pointf z=raw_objects[i].p[3];
			unsigned c=raw_objects[i].color;
			Vector O,X,Y,Z;
			makeVector(o.x,o.y,o.z,O);
			makeVector(x.x,x.y,x.z,X);
			makeVector(z.x,z.y,z.z,Y);
			makeVector(y.x,y.y,y.z,Z);
			printf("box:\n");
			printf("    ");debugVector(O);
			printf("    ");debugVector(X);
			printf("    ");debugVector(Y);
			printf("    ");debugVector(Z);
			Color color={((palette_colors[c]>>16)&255)/255.0,((palette_colors[c]>>8)&255)/255.0,(palette_colors[c]&255)/255.0};
			renderBox(O,X,Y,Z,color);
		}//box
		OBJTot=cnt;
	}
}

int frame_cnt = 0;

void render(int row_start, int row_cnt) {
	
	VSYNC(1);
	
	while (!RENDER_START())
		usleep(100);
	
	//printf("Rendering frame %d [%d, %d)...\n", frame_cnt++, row_start, row_start + row_cnt);
	VSYNC(0);
	
	sync_objects();
	
	VSYNC(1);
	usleep(0);
}

#endif