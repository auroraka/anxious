//#define MYLOCAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "basic.h"
#include "vector.h"
#include "object.h"
#include "color.h"

Color BG_C;//backgroung_color
Color LI_C;//light_color
Vector LI_O;

#ifdef MYLOCAL
#include <time.h>
#include "bmp.h"
#endif

//******************** SDRAM **************************************************/
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
//**************************** End SDRAM ************************************************/

//*************************** Camera ********************************/


struct Camera {
	Vector O, N, Dx, Dy;
	float lens_W, lens_H;
}CAM;

void Caminit() {
	makeVector(-2, 2, 0.5, CAM.O);
	makeVector(0.6, 1, -0.6, CAM.N);
	CAM.lens_H = 0.75;
	CAM.lens_W = 1;
	Vector tmp;
	normal(CAM.N, CAM.N);
	GetAnVerticalVector(CAM.N, CAM.Dx);
	mul(CAM.Dx, CAM.N, CAM.Dy);
	muld(CAM.Dx, CAM.lens_W, tmp);
	divd(tmp, 2, CAM.Dx);
	muld(CAM.Dy, CAM.lens_H, tmp);
	divd(tmp, 2, CAM.Dy);

}

void CamlookAt(float i, float j, Vector ret) {
	Vector tmp, tmp2;
	muld(CAM.Dy, (2 * i / PIC_H - 1), tmp);
	add(CAM.N, tmp, tmp);
	muld(CAM.Dx, (2 * j / PIC_W - 1), tmp2);
	add(tmp, tmp2, ret);
}

//********************** EndCamera **************************************/





//****************** Sphere *************************************/


struct Sphere {
	Color color;
	float diff, spec;
	struct IntersectResult irst;
	Vector O, De, Dc;
	float R;
}SP[2];
int SPTot = 0;

void SpInit() {
	SPTot = 0;

	makeVector(1, 5.5, -1.5, SP[0].O);
	SP[0].R = 0.4;
	makeColor(0, 1, 0, SP[0].color);
	SP[0].diff = 0.4;
	SP[0].spec = 0.8;
	makeVector(0, 0, 1, SP[0].De);
	makeVector(0, 1, 0, SP[0].Dc);
	SPTot++;
}

int intersectSphere(int n_sp, Vector ray_O, Vector ray_V) {
	normal(ray_V, ray_V);
	Vector P;
	sub(ray_O, SP[n_sp].O, P);
	Vector nP;
	copyVector(P, nP);
	neg(nP);
	float b = dot(nP, ray_V);
	float det = b * b - len2(P) + SP[n_sp].R * SP[n_sp].R;

	if (det > EPS) {
		det = sqrt(det);
		float x1 = b - det, x2 = b + det;

		if (x2 < EPS) return 0;
		if (x1 > EPS) {
			SP[n_sp].irst.dist = x1;
			SP[n_sp].irst.front = 1;
		}
		else {
			SP[n_sp].irst.dist = x2;
			SP[n_sp].irst.front = 0;
		}
	}
	else
		return 0;

	Vector tmp;
	muld(ray_V, SP[n_sp].irst.dist, tmp);
	add(ray_O, tmp, SP[n_sp].irst.C);
	sub(SP[n_sp].irst.C, SP[n_sp].O, tmp);
	normal(tmp, SP[n_sp].irst.N);

	if (SP[n_sp].irst.front == 0) {
		neg(SP[n_sp].irst.N);
	}
	return 1;
}

int intersectWithSphere(Vector ray_O, Vector ray_V) {
	float d = 1e30; int ret=-1;
	for (int i = 0; i < SPTot;i++) {
		if (intersectSphere(i, ray_O, ray_V)) {
			if (SP[i].irst.dist<d){
				d = SP[i].irst.dist;
				ret = i;
			}
		}
	}
	//return -1;
	return ret;
}

//**************************End Sphere ***************************/



void rayTracing(Vector ray_O, Vector ray_V,Color ret) {
	int n_sp = intersectWithSphere(ray_O, ray_V);

	if (n_sp!=-1) {
		if (SP[n_sp].diff > EPS || SP[n_sp].spec > EPS) {
			Color color, tmp;
			copyColor(SP[n_sp].color, color);

			mulc(color, BG_C, tmp);
			mulcd(tmp, SP[n_sp].diff, ret);

			Vector R;
			sub(LI_O, SP[n_sp].irst.C, R);
			normal(R, R);
			float RN = dot(R, SP[n_sp].irst.N);
			if (RN > EPS) {

				if (SP[n_sp].diff > EPS) {
					float diff = SP[n_sp].diff * RN;
					mulcd(color, diff, tmp);
					addc(ret, tmp, ret);
				}
				if (SP[n_sp].spec > EPS) {
					float spec = SP[n_sp].spec * pow(RN, SPEC_POWER);
					mulcd(color, spec, tmp);
					addc(ret, tmp, ret);
				}
			}

			normalize(ret);
			return;
		}
	}
	makeColor(0, 0, 0, ret);
}

void createScene() {
	//BG_Color
	makeColor(0.2, 0.2, 0.2, BG_C);

	//light
	makeVector(3, 3, 3, LI_O);
	makeColor(0.7, 0.7, 0.7, LI_C);

	//Bmp
#ifdef MYLOCAL
	Initialize(&BMP);
#endif

	//cam
	Caminit();

	//sphere
	SpInit();

	//plane
	//obj = new Plane;
	//obj->makeDemo(5);
	//objs.push_back(obj);


}


void run() {
	createScene();

	Vector ray_O;Vector ray_V;Color color;


	copyVector(CAM.O, ray_O);
	
	
	int ttt = 0;
	//clock_t tic=clock();
	
	while (1) {

		for (int i = 0; i < PIC_H; i++) {
			if (i % 10 == 0) printf("%d/%d\n", i, H);
			for (int j = 0; j < PIC_W; j++) {
				CamlookAt(i, j, ray_V);
				rayTracing(ray_O, ray_V,  color);
#ifdef MYLOCAL
				SetColor(&BMP, i, j, color);
#else
				if (isZero(color)){
					WRITE_TRANS(i, j);	
				}else{
					WRITE_PIXF(i, j, color[0],color[1],color[2]);
				}
#endif
			}
		}

#ifdef MYLOCAL
		char name[] = "outputa.bmp";
		//name[6] = ttt++;
		Output(&BMP, name);
		//if (ttt > 'a'+10) break;
#endif
		printf("%d\n", ttt++);
		SP[0].O[0] += 0.1F;
		break;
		//if (ttt >= 10) break;
	}

	//clock_t toc = clock();
	//printf("time: %.2lf\n", (double)(toc - tic) / CLOCKS_PER_SEC);
	//system("pause");
}

int main() {
#ifndef MYLOCAL
	configure_sccb();
	clean_sdram();
	render_bank = *render_buffer_port;
#endif
	run();
	return 0;
}