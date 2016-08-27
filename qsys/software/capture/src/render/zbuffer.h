#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "basic.h"
#include "color.h"

#ifdef MYLOCAL
#include "bmp.h"
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

#endif



#define MAX_B (10000000)

#ifndef MYLOCAL
#else
static int ZBuffer[PIC_H][PIC_W];
float ZBuffer_R(int i, int j) {
	return ZBuffer[i][j];
}
void ZBuffer_W(int i, int j, int z) {
	ZBuffer[i][j] = z;
}
#endif

void _setColor(int i, int j, Color color) {
#ifdef MYLOCAL
	_SetColor(&BMP, i, j, color);
#else
	int render_port=RENDER_PORT();
	if (isZero(color)) {
		WRITE_TRANS(i, j);
	}
	else {
		WRITE_PIXF(i, j, color[0], color[1], color[2]);
	}
#endif
}
void setColorXY(int x, int y, Color color) {
	_setColor(y, x, color);
}

void bufferColor(int i, int j, Color color, float z) {
	int x = z * 1000000;
	//if (x < ZBuffer_R(i, j)) {
		//ZBuffer_W(i, j, x);
		setColorXY(i, j, color);
	//}
}
void initBuffer() {
	for (int i = 0; i < PIC_H; i++) {
		for (int j = 0; j < PIC_W; j++) {
			//ZBuffer_W(i, j, MAX_B);
		}
	}
}

#endif