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

#define HEIGHT (480)
#define WIDTH (640)
#define PIC_H HEIGHT
#define PIC_W WIDTH

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


#endif //MEMORY_H
