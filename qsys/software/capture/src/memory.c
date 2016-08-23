//
// Created by Kanari on 2016/8/24.
//

#include "memory.h"

void clean_render() {
	int i, j, bank;
	for (bank = 0; bank < 3; ++bank) {
		for (j = 0; j < HEIGHT; ++j) {
			for (i = 0; i < WIDTH; ++i)
				IOWR(SDRAM, (1 << 23) | (bank << 19) | ((j) << 10) | (i), (1 << 24));
		}
	}
}