
#ifndef CPU_ID
#define CPU_ID 3
#endif

#if CPU_ID > 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

#include "basic.h"
#include "vector.h"
#include "object.h"

#include "../render.h"
#include "../memory.h"
#include "../common.h"


#include "renderer.h"

void render_init(int row_start, int row_cnt) {
}

void sync_objects() {
	int i;
	RawSphere sphere;
	int SPTot = OBJECT_CNT_R();
	for (i = 0; i < SPTot; ++i) {
		get_sphere(i, &sphere);
		printf("%d %d %d\n",(int)sphere.x, (int)sphere.y,(int)sphere.radius);
		int x=sphere.x,y=sphere.y,r=sphere.radius;
		if (x-r<0 || x+r>=PIC_W || y-r<0 || y+r>=PIC_H) continue;
		Pos2 p={x,y};
		Color color={0,0xbc/255.0,0xd4/255.0};
		drawSphereC(p,r,color);
	}
}

int frame_cnt = 0;

void render(int row_start, int row_cnt) {
	
	VSYNC(1);
	
	while (!RENDER_START())
		usleep(100);
	
	printf("Rendering frame %d [%d, %d)...\n ", frame_cnt++, row_start, row_start + row_cnt);
	VSYNC(0);
	
	sync_objects();
	Pos2 p={240,240};
	Color color={0,0xbc/255.0,0xd4/255.0};
	drawSphereC(p,10,color);
	
	VSYNC(1);
	usleep(0);
	
	usleep(1000*1000*3);
}

#endif