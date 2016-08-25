//#define MYLOCAL
#define CAM_IN_O
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "basic.h"
#include "vector.h"
#include "object.h"
#include "color.h"

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
	Vector O, N;
	float Dx, Dy;
}CAM;


void CamlookAt(int i, int j, Vector ret) {
	ret[0] = CAM.Dx*(2.0 * j / PIC_W - 1)*80;
	ret[1] = CAM.Dy*(2.0 * i / PIC_H - 1)*80;
	ret[2] = CAM.N[2]*80;
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

static cnt = 0;
int intersectSphere(int n_sp, Vector ray_O, Vector ray_V) {
	//printf("%.2lf %.2lf %.2lf\n", ray_V[0], ray_V[1], ray_V[2]);
	normal(ray_V, ray_V);
#ifdef CAM_IN_O
	if (SP[n_sp].O[2] < 0) {
		float x = ray_V[0] * (-SP[n_sp].O[2]) - SP[n_sp].O[0];
		if (fabs(x) > SP[n_sp].R*1.2)
			return 0;
		float y = ray_V[1] * (-SP[n_sp].O[2]) - SP[n_sp].O[1];
		if (fabs(y) > SP[n_sp].R*1.2) return 0;
	}
#endif
	Vector P,nP,tmp;
	float b, det;
	cnt++;
	sub(ray_O, SP[n_sp].O, P);
	copyVector(P, nP);
	neg(nP);
	//printf("%.2lf %.2lf %.2lf\n", nP[0], nP[1], nP[2]);
	//nP[0] = (int)nP[0];
	//nP[1] = (int)nP[1];
	//nP[2] = (int)nP[2];

	b = dot(nP, ray_V);
	b = (int)b;
	//printf("%.2lf\n", b);
	//printf("%.2lf %.2lf %.2lf | %.2lf %.2lf %.2lf \n", nP[0], nP[1], nP[2], ray_V[0], ray_V[1], ray_V[2]);
	det = b * b - len2(P) + SP[n_sp].R * SP[n_sp].R;
	//printf("%.2lf\n", det);
	if (det > EPS) {
		det = sqrt(det);
		float x1 = b - det, x2 = b + det;
		//printf("%.2lf %.2lf\n", x1, x2);
		if (x2 < EPS) return 0;
		if (x1 < EPS) return 0;
		SP[n_sp].irst.dist = x1;
	}
	else {
		return 0;
	}

	muld(ray_V, SP[n_sp].irst.dist, tmp);
	add(ray_O, tmp, SP[n_sp].irst.C);
	sub(SP[n_sp].irst.C, SP[n_sp].O, tmp);
	normal(tmp, SP[n_sp].irst.N);
	return 1;
}

int intersectWithSphere(Vector ray_O, Vector ray_V) {
	float d = 1e30; int ret = -1;
	int i;
	for (i = 0; i < SPTot; i++) {
		if (intersectSphere(i, ray_O, ray_V)) {
			if (SP[i].irst.dist<d) {
				d = SP[i].irst.dist;
				ret = i;
			}
		}
	}
	//return -1;
	return ret;
}

//**************************End Sphere ***************************/



void rayTracing(Vector ray_O, Vector ray_V, Color ret) {
	//printf("%.2lf %.2lf %.2lf\n\n", ray_V[0], ray_V[1], ray_V[2]);
	int n_sp = intersectWithSphere(ray_O, ray_V);

	if (n_sp != -1) {
		if (SP[n_sp].diff > EPS || SP[n_sp].spec > EPS) {
			Color color, tmp;
			copyColor(SP[n_sp].color, color);

			copyColor(color, tmp);
			mulcd(tmp, SP[n_sp].diff, ret);

			Vector R;
			sub(LI_O, SP[n_sp].irst.C, R);
			//printf("%.2lf %.2lf %.2lf\n", R[0], R[1], R[2]);
			float l = len(R);
			normal(R, R);
			float RN = dot(R, SP[n_sp].irst.N);
			//printf("%.2lf\n", RN);
			//printf("%.2lf\n", SP[n_sp].irst.N[0], SP[n_sp].irst.N[1], SP[n_sp].irst.N[2]);
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
	//Bmp
#ifdef MYLOCAL
	Initialize(&BMP);
#endif

	//light
	makeVector(40 , 40 , 30, LI_O);
	
	//cam
	makeVector(0, 0, 0, CAM.O);
	makeVector(0, 0, -1, CAM.N);
	CAM.Dx = 4;
	CAM.Dy = 3;

	//sphere
	SPTot = 0;
	makeVector(0, 0, -100, SP[0].O);
	SP[0].R = 50;
	makeColor(0, COLOR_MAX, 0, SP[0].color);
	SP[0].diff = 0.4;
	SP[0].spec = 0.25;
	makeVector(0, 0, 1, SP[0].De);
	makeVector(0, 1, 0, SP[0].Dc);
	SPTot++;

}

int getXY(int* x, int* y,int *r) {
	if (SP[0].O[2] >0) {
		return 0;
	}
	float u = -SP[0].O[0] / SP[0].O[2], v = -SP[1].O[0] / SP[0].O[2];
	if (-CAM.Dx <= u &&u <= CAM.Dx && -CAM.Dy <= v&&v <= CAM.Dy) {
		
		*x = (int)((u/ CAM.Dx+1)/2*PIC_W);
		*y = (int)((v / CAM.Dy + 1) / 2 * PIC_H);
		*r = (int)((-SP[0].R / SP[0].O[2] / CAM.Dx ) / 2 * PIC_W)+10;
		return 1;
	}
	else {
		return 0;
	}
}
void run() {
	Vector ray_V; Color color;
	int ttt = 0;
	int stt = 0;
	int i, j;
	createScene();

	int ox=0, oy=0, or=-1 ;
	while (1) {
		if (or != -1) {
			int xl, xr, yl, yr;
			if (oy - or < 0) yl = 0; else yl = oy - or;
			if (oy + or >= PIC_H) yr = PIC_W; else yr = oy + or;
			if (ox - or < 0) xl = 0; else xl = ox - or;
			if (ox + or >= PIC_W) xr = PIC_H; else xr = ox + or;
			for (i = yl; i < yr; i++) {
				for (j = xl; j < xr; j++) {
					#ifdef MYLOCAL
					makeColor(0, 0, 0, color);
					SetColor(&BMP, i, j, color);
					#else
					WRITE_TRANS(i, j);
					#endif
				}
			}
			or = -1;
		}
		cnt = 0;
		int x, y,r;
		if (getXY(&x, &y,&r)) {
			//printf("%d %d %d\n", x, y, r);
			//system("pause");
			int xl, xr, yl, yr;
			if (y - r < 0) yl = 0; else yl = y - r;
			if (y + r >= PIC_H) yr = PIC_H; else yr = y + r;
			if (x -r < 0) xl = 0; else xl = x - r;
			if (x + r >= PIC_W) xr = PIC_W;else xr = x + r;
			yl = 0; yr = PIC_H; xl = 0; xr = PIC_W;
			for (i = yl; i < yr; i++) {
				if (i % 10 == 0) printf("%d/%d\n", i, PIC_H);
				for (j = xl; j < xr; j++) {
					CamlookAt(i, j, ray_V);
					rayTracing(CAM.O, ray_V, color);

					#ifdef MYLOCAL
					SetColor(&BMP, i, j, color);
					#else
					if (isZero(color)) {
						WRITE_TRANS(i, j);
					}
					else {
						WRITE_PIXF(i, j, color[0], color[1], color[2]);
					}
					#endif
				}
			}
			ox = x; oy = y; or = r;
		}
		else {
			for (i = 0; i < PIC_H; i++) {
				for (j = 0; j < PIC_W; j++) {
					#ifdef MYLOCAL
					makeColor(0, 0, 0, color);
					SetColor(&BMP, i, j, color);
					#else
					WRITE_TRANS(i, j);
					#endif
				}
			}
		}

		printf("%d\n", ttt++);
		printf("cnt: %d\n", cnt);
		if (stt == 0) {
			SP[0].O[0] += 10;
			if (SP[0].O[0]>100) stt ^= 1;
		}
		else {
			SP[0].O[0] -= 0.1F;
			if (SP[0].O[0]<-100) stt ^= 1;
		}
#ifdef MYLOCAL
		char name[] = "outputa.bmp";
		name[6] = 'a' + ttt;
		Output(&BMP, name);
		if (ttt>10) break;
#endif
	}
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
