//#define CPU_ID 3

#if CPU_ID > 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>

#include "basic.h"
#include "vector.h"
#include "object.h"

#include "../render.h"
#include "../memory.h"

Color BG_C; // background color
Color LI_C; // light color
Vector LI_O;

#ifdef MYLOCAL
#include <time.h>
#include "bmp.h"
#endif

/**
 * Hardware communication helper macros
 */
#ifndef MYLOCAL

// RENDER_VSYNC_PIO_x_BASE
#define RENDER_VSYNC 0x08001020
 
#define RENDER_PORT() IORD(RENDER_PORT_PIO_BASE, 0)
#define RENDER_START() IORD(RENDER_START_PIO_BASE, 0)
#define VSYNC(x) IOWR(RENDER_VSYNC, 0, x)

#define WRITE_PIX(i, j, r, g, b) SDRAM_W(j, HEIGHT - i, ((r) << 16) | ((g) << 8) | (b))
#define WRITE_PIXF(i, j, r, g, b) SDRAM_W(j, HEIGHT - i, ((uchar)((r) * 255) << 16) | ((uchar)((g) * 255) << 8) | ((uchar)((b) * 255)))
#define WRITE_TRANS(i, j) SDRAM_CLEAR(j, HEIGHT - i)

#endif


/**
 * Camera definition and functions
 */
struct Camera {
	Vector O, N, Dx, Dy;
	float lens_W, lens_H;
} CAM;

void camInit() {
	makeVector(0, 0, 0, CAM.O);
	makeVector(0, 0, -1, CAM.N);
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

void camLookAt(float i, float j, Vector ret) {
	Vector tmp, tmp2;
	muld(CAM.Dy, (2 * i / HEIGHT - 1), tmp);
	add(CAM.N, tmp, tmp);
	muld(CAM.Dx, (2 * j / WIDTH - 1), tmp2);
	add(tmp, tmp2, ret);
}


/**
 * Object: Sphere
 */
struct Sphere {
	Color color;
	float diff, spec;
	struct IntersectResult irst;
	Vector O, De, Dc;
	float R;
} SP[2];
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
		} else {
			SP[n_sp].irst.dist = x2;
			SP[n_sp].irst.front = 0;
		}
	} else
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
	float d = 1e30;
	int ret = -1, i;
	for (i = 0; i < SPTot; i++) {
		if (intersectSphere(i, ray_O, ray_V)) {
			if (SP[i].irst.dist < d) {
				d = SP[i].irst.dist;
				ret = i;
			}
		}
	}
	return ret;
}


/**
 * Rendering functions
 */
void rayTracing(Vector ray_O, Vector ray_V, Color ret) {
	int n_sp = intersectWithSphere(ray_O, ray_V);
	
	if (n_sp != -1) {
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
	// BG_Color
	makeColor(0.2, 0.2, 0.2, BG_C);
	
	// light
	makeVector(3, 3, 3, LI_O);
	makeColor(300, 300, 400, LI_C);
	
	//Bmp
#ifdef MYLOCAL
	Initialize(&BMP);
#endif
	
	// Camera
	camInit();
	
	// Sphere
	SpInit();
}

void render_init(int row_start, int row_cnt) {
	createScene();
}

void sync_objects() {
	int i;
	volatile RawSphere sphere;
	SPTot = OBJECT_CNT_R();
	for (i = 0; i < SPTot; ++i) {
		sphere = get_sphere(i);
		makeVector(sphere.x, sphere.y, sphere.z, SP[i].O);
		SP[i].R = sphere.radius;
		makeColor(1, 1, 1, SP[i].color);
		SP[i].diff = 0.4;
		SP[i].spec = 0.8;
		makeVector(0, 0, 1, SP[i].De);
		makeVector(0, 1, 0, SP[i].Dc);
	}
}

int frame_cnt = 0;

void render(int row_start, int row_cnt) {
	int render_port, i, j;
	Vector ray_O;
	Vector ray_V;
	Color color;
	
	VSYNC(1);
	
	while (!RENDER_START())
		usleep(100);
	
	printf("Rendering frame %d [%d, %d)... ", frame_cnt++, row_start, row_start + row_cnt);
	VSYNC(0);
	
	render_port = RENDER_PORT();
	copyVector(CAM.O, ray_O);
	
	sync_objects();
	
	for (i = row_start; i < row_start + row_cnt; i++) {
		if ((i - row_start) % 10 == 9) printf("#");
		for (j = 0; j < WIDTH; j++) {
			camLookAt(i, j, ray_V);
			rayTracing(ray_O, ray_V, color);
#ifdef MYLOCAL
			SetColor(&BMP, i, j, color);
#else
			if (isZero(color)) WRITE_TRANS(i, j);
			else WRITE_PIXF(i, j, color[0], color[1], color[2]);
#endif
		}
	}
	printf("\n");

#ifdef MYLOCAL
	char name[] = "output.bmp";
	Output(&BMP, name);
#endif
	
	VSYNC(1);
}

#endif