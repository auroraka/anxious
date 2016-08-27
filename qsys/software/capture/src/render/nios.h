#ifndef NIOS_H
#define NIOS_H

#ifndef MYLOCAL


#include "sccb.h"
#include <system.h>
#include <unistd.h>
#include <io.h>
#define RAW_HEIGHT (480)
#define RAW_WIDTH (640)
#define WIDTH (RAW_WIDTH >> 1)
#define HEIGHT (RAW_HEIGHT >> 1)
const unsigned MASK_WIDTH = WIDTH >> 5;

volatile unsigned *sdram = (unsigned *)SDRAM_CONTROLLER_0_BASE;
unsigned *model = (unsigned *)MODEL_MEMORY_BASE;
unsigned *frame = (unsigned *)RECOG_CPU_MEMORY_BASE;
unsigned *row = (unsigned *)RECOG_CPU_MEMORY_BASE + (WIDTH * HEIGHT >> 5);
unsigned *render_buffer_port = (unsigned *)RENDER_BUFFER_PORT_PIO_BASE;
unsigned *render_vsync = (unsigned *)RENDER_VSYNC_PIO_BASE;
unsigned *cam_buffer_port = (unsigned *)CAM_BUFFER_PORT_PIO_BASE;

unsigned cam_bank, render_bank;

#define SDRAM_R(x, y) sdram[(cam_bank << 19) | ((y) << 10) | (x)]
#define SDRAM_W(x, y, val) (sdram[(1 << 23) | (render_bank << 19) | ((y) << 10) | (x)] = (val))
#define SDRAM_CLEAR(x, y) (sdram[(1 << 23) | (render_bank << 19) | ((y) << 10) | (x)] = (1 << 24))
#define get_frame(x, y) ((frame[(y) * MASK_WIDTH + ((x) >> 5)] >> ((x) & 31)) & 1)

#define WRITE_PIX(i,j,r,g,b) SDRAM_W(j,PIC_H-i,(r<<16)|(g<<8)|(b))
#define WRITE_PIXF(i,j,r,g,b) SDRAM_W(j,PIC_H-i,((uchar)(r*255)<<16)|((uchar)(g*255)<<8)|((uchar)(b*255)))
#define WRITE_TRANS(i,j) SDRAM_W(j,PIC_H-i,1<<24)

void clean_sdram() {

	int i, j, bank;
	for (bank = 0; bank < 3; ++bank) {
		for (j = 0; j < 480; ++j) {
			for (i = 0; i < 640; ++i)
				sdram[(1 << 23) | (bank << 19) | ((j) << 10) | (i)] = (1 << 24);
		}
		printf("Cleaned bank %d\n", bank);
	}
}

#endif


#endif //NIOS_H