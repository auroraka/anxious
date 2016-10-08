#ifndef ZBUFFER_H
#define ZBUFFER_H

#include "basic.h"
#include "color.h"
#include "../common.h"
#include "../memory.h"

//render: bank1
static unsigned render_port;
#define RENDER_PORT() IORD(RENDER_PORT_PIO_BASE, 0)
#define SDRAM_RENDER_W(x, y, val) (IOWR(SDRAM,(1 << 23) | (render_port << 19) | ((y) << 10) | (x), (val)))
#define SDRAM_RENDER_CLEAR(x, y) SDRAM_RENDER_W(x, y, TRANSPARENT)
#define WRITE_PIX(i, j, r, g, b) SDRAM_RENDER_W(j, HEIGHT - i, ((r) << 16) | ((g) << 8) | (b))
#define WRITE_PIXF(i, j, r, g, b) SDRAM_RENDER_W(j, HEIGHT - i, ((uchar)((r) * 255) << 16) | ((uchar)((g) * 255) << 8) | ((uchar)((b) * 255)))
#define WRITE_TRANS(i, j) SDRAM_RENDER_CLEAR(j, HEIGHT - i)


//buffer -> (x,y)~(640,480)
#define ZBuffer_W(x,y,val) IOWR(SDRAM,(3 << 23) | (x<<10)|y, val)
#define ZBuffer_R(x,y) IORD(SDRAM,(3 << 23) | (x<<10)|y)

#define MAX_B (10000000)

#ifndef isZero
#define isZero(A) (fabs(A[0]) < EPS && fabs(A[1]) < EPS && fabs(A[2]) < EPS)
#endif 


void _setColor(int i, int j, Color color);
void setColorXY(int x, int y, Color color);
void bufferColor(int i, int j, Color color, float z);
void initZBuffer();


#endif
