//
// Created by Kanari on 2016/8/24.
//

#ifndef ANXIOUS_MEMORY_H
#define ANXIOUS_MEMORY_H

#include <system.h>
#include <io.h>
#include "../generated_bsp_0/system.h"
#include "../generated_bsp_0/HAL/inc/io.h"

#define HEIGHT (480)
#define WIDTH (640)

static volatile unsigned *SDRAM = ((unsigned *)SDRAM_CONTROLLER_0_BASE);

//#define SDRAM_R(x, y) (IORD(SDRAM,(cam_port << 19) | ((y) << 10) | (x)))
//#define SDRAM_W(x, y, val) (IOWR(SDRAM,(1 << 23) | (render_port << 19) | ((y) << 10) | (x), (val)))
#define SDRAM_R(x, y) (SDRAM[(cam_port << 19) | ((y) << 10) | (x)])
#define SDRAM_W(x, y, val) (SDRAM[(1 << 23) | (render_port << 19) | ((y) << 10)| (x)]= (val))
#define SDRAM_CLEAR(x, y) SDRAM_W(x, y, TRANSPARENT)

static volatile unsigned *SHARED_MEMORY = ((unsigned *)SHARED_MEMORY_BASE);

//#define SHARED_R(x) (SHARED_MEMORY[x])
//#define SHARED_W(x, val) (SHARED_MEMORY[x] = val)

#define SHARED_R(x) (IORD((SHARED_MEMORY_BASE),(x)))
#define SHARED_W(x, val) (IOWR((SHARED_MEMORY_BASE),(x),(val)))

void clean_sdram(unsigned bank);

#endif //MEMORY_H
