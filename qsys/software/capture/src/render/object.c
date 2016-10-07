//
// Created by Kanari on 2016/8/24.
//

#include "object.h"
#include "../memory.h"
#include "zbuffer.h"

void reset_objects() {
	int i,j;
	for (i=0;i<OBJECT_TOT;i++){
		for (j=0;j<OBJECT_LENGTH;j++){
			OBJECT_W(i,j,0);
		}
	}
	RENDER_STATUS_W(RENDER_IDLE);
	OBJECT_CNT_W(0);
	initZBuffer();
	clean_sdram(1);
	printf("[Object] delete all objects\n");
	printf("[Object] now-tot: %d\n",OBJECT_CNT_R());
}

void remove_object(int idx) {
}

void printPoint(pointf x){
	printf("(%d,%d,%d) ",(int)x.x,(int)x.y,(int)x.z);
	
}
void printSendSphere3d(pointf pf[],unsigned color){
	printf("[Object] send-cube3d: ");
	printf("o=");printPoint(pf[0]);
	printf("x=");printPoint(pf[1]);
	printf("color=%u ",color);
	printf("\n");	
}
void add_sphere3d(pointf *pf, float r, unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		printf("[Object] add-sphere3d failed: not ready\n");
		return;
	}
	int cnt=OBJECT_CNT_R();
	int i;IntF x;
	for (i=0;i<2;i++){
		x.f=pf[i].x; OBJECT_W(cnt+1,i*3+0,x.u); 		
		x.f=pf[i].y; OBJECT_W(cnt+1,i*3+1,x.u); 		
		x.f=pf[i].z; OBJECT_W(cnt+1,i*3+2,x.u); 		
	}
	OBJECT_W(cnt+1,6,color);
		
	RENDER_STATUS_W(RENDER_ADD_SPHERE3D);
	printf("[Object] now-tot: %d\n",cnt);
	printSendSphere3d(pf,color);
}
void add_sphere2d(int x,int y,int r,unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		printf("[Object] add-sphere failed: not ready\n");
		return;
	}
	if (x-r<0 || x+r>=640 || y-r<0 || y+r>=480){
		printf("[Object] add-sphere failed: out of canvas\n");		
		return;
	}
	int cnt=OBJECT_CNT_R();
	OBJECT_W(cnt+1,0,640-x);
	OBJECT_W(cnt+1,1,480-y);
	OBJECT_W(cnt+1,2,r);
	OBJECT_W(cnt+1,3,color);
	
	RENDER_STATUS_W(RENDER_ADD_SPHERE);
	
	printf("[Object] now-tot: %d\n",cnt);
	printf("[Object] send-sphere: (%d,%d) r=%d\n", x,y,r);
}

void printSendCube(pointf pf[]){
	printf("[Object] send-cube: ");
	printf("o=");printPoint(pf[0]);
	printf("x=");printPoint(pf[1]);
	printf("y=");printPoint(pf[2]);
	printf("z=");printPoint(pf[3]);
	printf("\n");	
}
void add_cube(pointf pf[], unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		printf("[Object] add-cube failed: not ready\n");
		return;
	}
	int cnt=OBJECT_CNT_R();
	int i;IntF x;
	for (i=0;i<4;i++){
		x.f=pf[i].x; OBJECT_W(cnt+1,i*3+0,x.u); 		
		x.f=pf[i].y; OBJECT_W(cnt+1,i*3+1,x.u); 		
		x.f=pf[i].z; OBJECT_W(cnt+1,i*3+2,x.u); 		
	}
	OBJECT_W(cnt+1,12,color);
	
	
	RENDER_STATUS_W(RENDER_ADD_CUBE);
	printf("[Object] now-tot: %d\n",cnt);
	printSendCube(pf);
	
}

