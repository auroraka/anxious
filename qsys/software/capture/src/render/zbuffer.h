#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "basic.h"
#include "color.h"

#ifdef MYLOCAL
#include "bmp.h"
#else
#include "object.h"
#endif

#ifndef MYLOCAL

// RENDER_VSYNC_PIO_x_BASE
#define RENDER_VSYNC 0x08001010
 
#define RENDER_PORT() IORD(RENDER_PORT_PIO_BASE, 0)
#define RENDER_START() IORD(RENDER_START_PIO_BASE, 0)
#define VSYNC(x) IOWR(RENDER_VSYNC, 0, x)

#define WRITE_PIX(i, j, r, g, b) SDRAM_W(j, HEIGHT - i, ((r) << 16) | ((g) << 8) | (b))
#define WRITE_PIXF(i, j, r, g, b) SDRAM_W(j, HEIGHT - i, ((uchar)((r) * 255) << 16) | ((uchar)((g) * 255) << 8) | ((uchar)((b) * 255)))
#define WRITE_TRANS(i, j) SDRAM_CLEAR(j, HEIGHT - i)

//(x,y)~(640,480)
//#define BUFFER_R(x, y) IORD(SDRAM,((3<<23)| ((x)<<9) | (y)))
//#define BUFFER_W(x, y, val) IOWR(SDRAM,((3<<23)| ((x)<<10) | (y)), (val))

#define BUFFER_W(x,y,val) IOWR(SDRAM,(3 << 23) | (x<<10)|y, val)
#define BUFFER_R(x,y) IORD(SDRAM,(3 << 23) | (x<<10)|y)
	

#endif


static int render_port;
static int render_time_check;
static int offline_render_status;

#define MAX_B (10000000)

#ifndef MYLOCAL
#define ZBuffer_R(i,j) BUFFER_R(i,j)
#define ZBuffer_W(i,j,val) BUFFER_W(i,j,val)
#else
static int ZBuffer[PIC_H][PIC_W];
#define ZBuffer_R(i,j) ZBuffer[i][j]
#define ZBuffer_W(i,j,val) ZBuffer[i][j]=val
#endif

void _setColor(int i, int j, Color color) {
#ifdef MYLOCAL
	_SetColor(&BMP, i, j, color);
#else
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
#endif
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
void initBuffer() {
	render_port=RENDER_PORT();

	IntF inf;
	inf.f=1e30;
	for (int i = 0; i < PIC_W; i++) {
		for (int j = 0; j < PIC_H; j++) {
			ZBuffer_W( i,j ,0);
		}
	}
}

#endif