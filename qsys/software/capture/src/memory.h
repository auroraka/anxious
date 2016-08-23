//
// Created by Kanari on 2016/8/24.
//

#ifndef MEMORY_H
#define MEMORY_H

#include <system.h>
#include <io.h>
#include "../generated_bsp_0/system.h"
#include "../generated_bsp_0/HAL/inc/io.h"

#define HEIGHT (480)
#define WIDTH (640)

#define SDRAM SDRAM_CONTROLLER_0_BASE
#define SHARED_MEMORY SHARED_MEMORY_BASE
#define KEY KEY_PIO_BASE

#define SDRAM_R(x, y) IORD(SDRAM, (cam_bank << 19) | ((y) << 10) | (x))
#define SDRAM_W(x, y, val) IOWR(SDRAM, (1 << 23) | (render_port << 19) | ((y) << 10) | (x), (val))
#define SDRAM_CLEAR(x, y) IOWR(SDRAM, (1 << 23) | (render_port << 19) | ((y) << 10) | (x), (1 << 24))

#define SHARED_R(x) IORD(SHARED_MEMORY, x)
#define SHARED_W(x, val) IOWR(SHARED_MEMORY, x, val)

#define KEY_R() IORD(KEY, 0)

void clean_render();

#endif //MEMORY_H
