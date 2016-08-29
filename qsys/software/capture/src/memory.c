//
// Created by Kanari on 2016/8/24.
//


#include "memory.h"
#include "common.h"

void clean_sdram(unsigned bank) {
	int i, j, port;
	for (port = 0; port < 3; ++port) {
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
				SDRAM[(bank << 23) | (port << 19) | ((j) << 10) | (i)] = TRANSPARENT;
		}
	}
}

