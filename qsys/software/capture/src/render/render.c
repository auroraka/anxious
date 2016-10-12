
#ifndef CPU_ID
#define CPU_ID 3
#endif

#if CPU_ID > 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

#include "../debug.h"
#include "../memory.h"
#include "../common.h"
#include "../palette.h"

#include "render.h"
#include "object.h"
#include "renderer.h"

#define RENDER_VSYNC RENDER_VSYNC_PIO_0_BASE
#define RENDER_START() IORD(RENDER_START_PIO_BASE, 0)
#define VSYNC(x) IOWR(RENDER_VSYNC, 0, x)


void render_init(int row_start, int row_cnt) {
	initZBuffer();
}

inline void debugVector(Vector A){
	sprintf(MSG,"%d %d %d\n",(int)A[0],(int)A[1],(int)A[2]);
	debugMSG();
}

void printVector(Vector v){
	sprintf(MSG,"(%d,%d,%d) ",(int)v[0],(int)v[1],(int)v[2]);
	debugMSG();
}
void printReceiveCube(Vector V[]){
	sprintf(MSG,"[Object] receive-cube: ");
	debugMSG();
	int i;
	for (i=0;i<4;i++) printVector(V[i]);
	sprintf(MSG,"\n");
	debugMSG();
}
void printReceiveSphere3D(Vector V[],unsigned color){
	sprintf(MSG,"[Object] receive-sphere3d: ");
	debugMSG();
	int i;
	for (i=0;i<2;i++) printVector(V[i]);
	sprintf(MSG,"color=%u\n",color);
	debugMSG();
}

int ttt=0;
void sync_objects() {	
	int status=RENDER_STATUS_R();
	
	ttt++;
	if (ttt%5000==0){
		sprintf(MSG,"sync_objects, RENDER_STATUS=%d\n\0",status);
		debugMSG();
	}
	if (status==RENDER_IDLE){
		return;
	}
	else if (status==RENDER_CLEAR){
		RENDER_STATUS_W(RENDER_IDLE);
		return;
	}
	else if (status ==RENDER_ADD_CUBE){		
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
		sprintf(MSG,"[Object] now-tot: %d\n",cnt);
		debugMSG();
		sprintf(MSG,"[Object] render-cube: doing\n");
		debugMSG();
		if (!renderBox(V[0],V[1],V[2],V[3],color)){
			sprintf(MSG,"[Object] render-cube failed: out of cavans\n");
			debugMSG();
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
		sprintf(MSG,"[Object] sphere-color: %d %d %d\n",(int)color[0],(int)color[1],(int)color[2]);
		debugMSG();
		printReceiveSphere3D(V,c);
		sprintf(MSG,"[Object] now-tot: %d\n",cnt);
		debugMSG();
		sprintf(MSG,"[Object] render-sphere3d: doing\n");
		debugMSG();
		if (!renderSphere3d(V[0],V[1],color)){
			sprintf(MSG,"[Object] render-sphere3d failed: out of cavans\n");
			debugMSG();
		}
		OBJECT_CNT_W(cnt+1);
		RENDER_STATUS_W(RENDER_IDLE);
	}else{
		sprintf(MSG,"[Object] format error\n");
		debugMSG();
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