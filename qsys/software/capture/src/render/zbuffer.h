#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "basic.h"
#include "color.h"
#include "vector.h"
#include "../memory.h"


static int render_time_check;
static int offline_render_status;

#define MAX_B (10000000)


void _setColor(int i, int j, Color color) {
	if (isZero(color)) {
		WRITE_TRANS(i, j);
	}
	else {
		WRITE_PIXF(i, j, color[0], color[1], color[2]);
	}
	render_time_check++;
	if (render_time_check>=10){
		render_time_check=0;
		offline_render_status=RENDER_STATUS_R();
	}
}
void setColorXY(int x, int y, Color color) {
	_setColor(y, x, color);
}

void bufferColor(int i, int j, Color color, float z) {
	IntF buf;
	buf.u=ZBuffer_R(i, j);
	if (z > buf.f) {
		buf.f=z;
		ZBuffer_W(i, j, buf.u);
		setColorXY(i, j, color);
	}else{
		//printf("%d %d [%d %d]\n",x,ZBuffer_R(i, j),i,j);
	}
}

#endif