//
// Created by Kanari on 2016/8/24.
//

#ifndef ANXIOUS_MEMORY_H
#define ANXIOUS_MEMORY_H

#include <system.h>
#include <io.h>
#include "../generated_bsp_0/system.h"
#include "../generated_bsp_0/HAL/inc/io.h"

//trans float to unsigned to store in memory
typedef union{
	float f;
	unsigned u;
}IntF;



static volatile unsigned *SDRAM = ((unsigned *)SDRAM_CONTROLLER_0_BASE);

//clean the hole bank
void clean_sdram(unsigned bank);

//get port for overlay
#define BUFFER_PORT() IORD(OVERLAY_PORT_PIO_BASE, 0)


//SHARED_MEMORY addr use:
//0: center_l (recognition)
//1: center_r (recognition)
//2: loc.x
//3: loc.y
//4: loc.z
//5: obj_cnt (render)
//6+ raw_objects[i] (render)
static volatile unsigned *SHARED_MEMORY = ((unsigned *)SHARED_MEMORY_BASE);
#define SHARED_R(x) (IORD((SHARED_MEMORY_BASE),(x)))
#define SHARED_W(x, val) (IOWR((SHARED_MEMORY_BASE),(x),(val)))


//render: bank1
static unsigned render_port;
#define SDRAM_RENDER_W(x, y, val) (IOWR(SDRAM,(1 << 23) | (render_port << 19) | ((y) << 10) | (x), (val)))
#define SDRAM_RENDER_CLEAR(x, y) SDRAM_RENDER_W(x, y, TRANSPARENT)
#define WRITE_PIX(i, j, r, g, b) SDRAM_RENDER_W(j, HEIGHT - i, ((r) << 16) | ((g) << 8) | (b))
#define WRITE_PIXF(i, j, r, g, b) SDRAM_RENDER_W(j, HEIGHT - i, ((uchar)((r) * 255) << 16) | ((uchar)((g) * 255) << 8) | ((uchar)((b) * 255)))
#define WRITE_TRANS(i, j) SDRAM_RENDER_CLEAR(j, HEIGHT - i)


//buffer -> (x,y)~(640,480)
#define ZBuffer_W(x,y,val) IOWR(SDRAM,(3 << 23) | (x<<10)|y, val)
#define ZBuffer_R(x,y) IORD(SDRAM,(3 << 23) | (x<<10)|y)
void initZBuffer();

#endif //MEMORY_H
