#if CPU_ID>=2

#include "zbuffer.h"

void _setColor(int i, int j, Color color) {
	if (isZero(color)) {
		WRITE_TRANS(i, j);
	}
	else {
		WRITE_PIXF(i, j, color[0], color[1], color[2]);
	}
}
void setColorXY(int x, int y, Color color) {
	_setColor(y, x, color);
}

void bufferColor(int i, int j, Color color, float z) {
	IntF buf;
	buf.u=ZBuffer_R(i, j);
	if (z < buf.f) {
		buf.f=z;
		ZBuffer_W(i, j, buf.u);
		setColorXY(i, j, color);
	}else{
	}

}

void initZBuffer() {
	render_port=RENDER_PORT();
	
	IntF inf;
	inf.f=1e30;
	int i,j;
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			ZBuffer_W( i,j ,inf.u);
		}
	}
}

#endif 