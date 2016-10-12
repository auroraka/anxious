//[CPU2],[CPU3]
#if CPU_ID>=2

#include "../debug.h"
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
	sprintf(MSG,"[Object] delete all objects\n");
	debugMSG();
	sprintf(MSG,"[Object] now-tot: %d\n",OBJECT_CNT_R());
	debugMSG();
}

void remove_object(int idx) {
}

void printPoint(pointf x){
	sprintf(MSG,"(%d,%d,%d) ",(int)x.x,(int)x.y,(int)x.z);
	debugMSG();	
}
void printSendSphere3d(pointf pf[],unsigned color){
	sprintf(MSG,"[Object] send-cube3d: ");
	debugMSG();
	sprintf(MSG,"o=");printPoint(pf[0]);
	debugMSG();
	sprintf(MSG,"x=");printPoint(pf[1]);
	debugMSG();
	sprintf(MSG,"color=%u ",color);
	debugMSG();
	sprintf(MSG,"\n");	
	debugMSG();
}
void add_sphere3d(pointf *pf, unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		sprintf(MSG,"[Object] add-sphere3d failed: not ready\n");
		debugMSG();
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
	sprintf(MSG,"[Object] now-tot: %d\n",cnt);
	debugMSG();
	printSendSphere3d(pf,color);
}
void add_sphere2d(int x,int y,int r,unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		sprintf(MSG,"[Object] add-sphere failed: not ready\n");
		debugMSG();
		return;
	}
	if (x-r<0 || x+r>=640 || y-r<0 || y+r>=480){
		sprintf(MSG,"[Object] add-sphere failed: out of canvas\n");		
		debugMSG();
		return;
	}
	int cnt=OBJECT_CNT_R();
	OBJECT_W(cnt+1,0,640-x);
	OBJECT_W(cnt+1,1,480-y);
	OBJECT_W(cnt+1,2,r);
	OBJECT_W(cnt+1,3,color);
	
	RENDER_STATUS_W(RENDER_ADD_SPHERE);
	
	sprintf(MSG,"[Object] now-tot: %d\n",cnt);
	debugMSG();
	sprintf(MSG,"[Object] send-sphere: (%d,%d) r=%d\n", x,y,r);
	debugMSG();
}

void printSendCube(pointf pf[],unsigned color){
	sprintf(MSG,"[Object] send-cube: ");
	debugMSG();
	sprintf(MSG,"o=");printPoint(pf[0]);
	debugMSG();
	sprintf(MSG,"x=");printPoint(pf[1]);
	debugMSG();
	sprintf(MSG,"y=");printPoint(pf[2]);
	debugMSG();
	sprintf(MSG,"z=");printPoint(pf[3]);
	debugMSG();
	sprintf(MSG,"color=%u ",color);
	debugMSG();
	sprintf(MSG,"\n");	
	debugMSG();
}
void printSendPyramid(pointf pf[],unsigned color){
	sprintf(MSG,"[Object] send-pyramid: ");
	debugMSG();
	sprintf(MSG,"o=");printPoint(pf[0]);
	debugMSG();
	sprintf(MSG,"x=");printPoint(pf[1]);
	debugMSG();
	sprintf(MSG,"y=");printPoint(pf[2]);
	debugMSG();
	sprintf(MSG,"a=");printPoint(pf[3]);
	debugMSG();
	sprintf(MSG,"color=%u ",color);
	debugMSG();
	sprintf(MSG,"\n");	
	debugMSG();
}
void add_cube(pointf pf[], unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		sprintf(MSG,"[Object] add-cube failed: not ready\n");
		debugMSG();
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
	sprintf(MSG,"[Object] now-tot: %d\n",cnt);
	debugMSG();
	printSendCube(pf,color);
	
}
void add_pyramid(pointf pf[], unsigned color) {
	int status=RENDER_STATUS_R();
	if (status!=RENDER_IDLE){
		sprintf(MSG,"[Object] add-pyramid failed: not ready\n");
		debugMSG();
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
	
	
	RENDER_STATUS_W(RENDER_ADD_PYRAMID);
	sprintf(MSG,"[Object] now-tot: %d\n",cnt);
	debugMSG();
	printSendPyramid(pf,color);
	
}

#endif 