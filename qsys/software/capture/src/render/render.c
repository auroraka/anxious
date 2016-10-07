
#ifndef CPU_ID
#define CPU_ID 3
#endif

#if CPU_ID > 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

#include "../memory.h"
#include "../common.h"
#include "../palette.h"

#include "render.h"
#include "object.h"
#include "renderer.h"

#define RENDER_VSYNC 0x08001010 
#define RENDER_START() IORD(RENDER_START_PIO_BASE, 0)
#define VSYNC(x) IOWR(RENDER_VSYNC, 0, x)


void render_init(int row_start, int row_cnt) {
	initZBuffer();
}

inline void debugVector(Vector A){
	printf("%d %d %d\n",(int)A[0],(int)A[1],(int)A[2]);
}

void printVector(Vector v){
	printf("(%d,%d,%d) ",(int)v[0],(int)v[1],(int)v[2]);
}
void printReceiveCube(Vector V[]){
	printf("[Object] receive-cube: ");
	int i;
	for (i=0;i<4;i++) printVector(V[i]);
	printf("\n");
}
void printReceiveSphere3D(Vector V[],unsigned color){
	printf("[Object] receive-sphere3d: ");
	int i;
	for (i=0;i<2;i++) printVector(V[i]);
	printf("color=%u\n",color);
}

int ttt=0;
void sync_objects() {	
	int status=RENDER_STATUS_R();
	
	ttt++;
	if (ttt%5000==0) printf("sync_objects, RENDER_STATUS=%d\n",status);

	if (status==RENDER_IDLE){
		return;
	}
	else if (status==RENDER_CLEAR){
		RENDER_STATUS_W(RENDER_IDLE);
		return;
	}
	else if (status == RENDER_ADD_SPHERE){
		int cnt=OBJECT_CNT_R();
		int x  =OBJECT_R(cnt+1,0);
		int y  =OBJECT_R(cnt+1,1);
		int r  =OBJECT_R(cnt+1,2);
		unsigned c  =OBJECT_R(cnt+1,3);
		printf("[Object] now-tot: %d\n",cnt);
		printf("[Object] receive-sphere: (%d,%d) r=%d\n", x,y,r);
		Pos2 p={x,y};
		Color color={((palette_colors[c]>>16)&255)/255.0,((palette_colors[c]>>8)&255)/255.0,(palette_colors[c]&255)/255.0};
		if (x-r<0 || x+r>=PIC_W || y-r<0 || y+r>=PIC_H){
			printf("[Object] render-sphere failed: out of cavans\n");
		}else{
			printf("[Object] render-sphere: doing\n");
			OBJECT_CNT_W(cnt+1);			
			drawSphereC(p,r,color);
		}
		RENDER_STATUS_W(RENDER_IDLE);
	}else if (status ==RENDER_ADD_CUBE){		
		int cnt=OBJECT_CNT_R();
		int i,j;IntF x;
		Vector V[4];
		for (i=0;i<4;i++){
			for (j=0;j<3;j++){
				x.u=OBJECT_R(cnt+1,i*3+j); 
				V[i][j]=x.f;
			}
		}
		unsigned c=OBJECT_R(cnt+1,12);
		Color color={((palette_colors[c]>>16)&255)/255.0,((palette_colors[c]>>8)&255)/255.0,(palette_colors[c]&255)/255.0};
		printReceiveCube(V);
		printf("[Object] now-tot: %d\n",cnt);
		printf("[Object] render-cube: doing\n");
		if (!renderBox(V[0],V[1],V[2],V[3],color)){
			printf("[Object] render-cube failed: out of cavans\n");
		}
		OBJECT_CNT_W(cnt+1);
		RENDER_STATUS_W(RENDER_IDLE);
	}else if (status ==RENDER_ADD_SPHERE3D){		
		int cnt=OBJECT_CNT_R();
		int i,j;IntF x;
		Vector V[2];
		for (i=0;i<2;i++){
			for (j=0;j<3;j++){
				x.u=OBJECT_R(cnt+1,i*3+j); 
				V[i][j]=x.f;
			}
		}
		unsigned c=OBJECT_R(cnt+1,6);
		Color color={((palette_colors[c]>>16)&255)/255.0,((palette_colors[c]>>8)&255)/255.0,(palette_colors[c]&255)/255.0};
		printf("[Object] sphere-color: %d %d %d\n",(int)color[0],(int)color[1],(int)color[2]);
		printReceiveSphere3D(V,c);
		printf("[Object] now-tot: %d\n",cnt);
		printf("[Object] render-sphere3d: doing\n");
		if (!renderSphere3d(V[0],V[1],color)){
			printf("[Object] render-sphere3d failed: out of cavans\n");
		}
		OBJECT_CNT_W(cnt+1);
		RENDER_STATUS_W(RENDER_IDLE);
	}

}

int frame_cnt = 0;

void render(int row_start, int row_cnt) {
	
	VSYNC(1);
	
	while (!RENDER_START())
		usleep(100);
	
	
	VSYNC(0);
	
	sync_objects();
	usleep(5000);
	
	VSYNC(1);
	usleep(0);
}

#endif