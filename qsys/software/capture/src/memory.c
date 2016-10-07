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

void initZBuffer() {
	//IntF inf;
	//inf.f=1e30;
	int i,j;
	for (i = 0; i < WIDTH; i++) {
		for (j = 0; j < HEIGHT; j++) {
			ZBuffer_W( i,j ,0);
		}
	}
}

int getRand(){
	return rand();
}
//带校验和的随机数用以检验内存
int getCheckNum(){
	int j;
	int x=getRand();
	if (x&1) x^=1;
	if ((x>>16)&1) x^=(1<<16);
	for (j=31;j>16;j--) if ((x>>j)&1) x^=(1<<16);
	for (j=15;j>0;j--) if ((x>>j)&1) x^=1;
	return x;
}
int checkNum(int x){
	int y=0,z=0,i;
	for (i=31;i>16;i--) if ((x>>i)&1) z^=1;
	for (i=15;i>0;i--) if ((x>>i)&1) y^=1;
	return (y==(x&1)) && (z==((x>>16)&1));
}