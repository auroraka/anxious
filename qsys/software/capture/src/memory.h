//
// Created by Kanari on 2016/8/24.
//

#ifndef MEMORY_H
#define MEMORY_H

#include <system.h>
#include <io.h>
#include "../generated_bsp_0/system.h"
#include "../generated_bsp_0/HAL/inc/io.h"

#include "palette.h"

#define HEIGHT (480)
#define WIDTH (640)

static volatile unsigned *SDRAM = ((unsigned *)SDRAM_CONTROLLER_0_BASE);
static volatile unsigned *SHARED_MEMORY = ((unsigned *)SHARED_MEMORY_BASE);
#define KEY KEY_PIO_BASE

#define SDRAM_R(x, y) (SDRAM[(cam_port << 19) | ((y) << 10) | (x)])
#define SDRAM_W(x, y, val) (SDRAM[(1 << 23) | (render_port << 19) | ((y) << 10) | (x)] = (val))
#define SDRAM_CLEAR(x, y) SDRAM_W(x, y, TRANSPARENT)

#define SHARED_R(x) (SHARED_MEMORY[x])
#define SHARED_W(x, val) (SHARED_MEMORY[x] = val)

#define KEY_R() IORD(KEY, 0)

void clean_sdram(unsigned bank);

#endif //MEMORY_H
