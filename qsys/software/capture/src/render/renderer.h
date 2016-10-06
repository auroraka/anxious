#ifndef RENDER_H
#define RENDER_H

#include "basic.h"
#include "vector.h"
#include "color.h"
#include "pos.h"
#include "transform.h"
#include "zbuffer.h"

static Color BG_C = { 0.5,0.5,0.5 };
static Vector LI_O = { 300,300,250 };

const static float focus_x_l = 1117.36809f, focus_y_l = 1115.59155f;
const static float focus_x_r = 1098.56402f, focus_y_r = 1095.74358f;
const static float center_x_l = 284.432591f, center_y_l = 205.206010f;
const static float center_x_r = 260.334008f, center_y_r = 307.355809f;
const static float stereo_dist = 17.6; // cm


void getColor(Vector _N, Vector C, Color _color, Color ret) {
	Vector N;
	copyVector(_N, N);
	normal(N, N);
	Color color, tmp;

	copyColor(_color, color);
	mulc(color, BG_C, tmp);
	mulcd(tmp, DIFF, ret);

	Vector R;
	sub(LI_O, C, R);
	normal(R, R);
	float RN = dot(R, N);
	//printf("%.5lf\n", RN);
	if (RN > EPS) {

		if (DIFF > EPS) {
			float diff = DIFF * RN;
			mulcd(color, diff, tmp);
			//printf("diff: "); debugVector(tmp);
			addc(ret, tmp, ret);
		}
		/*if (SPEC > EPS) {
			float spec = SPEC * pow(RN, SPEC_POWER);
			mulcd(color, spec, tmp);
			printf("spec: "); debugVector(tmp);
			addc(ret, tmp, ret);
		}*/
	}
	normalize(ret);
}
void getColorNR(Vector _N, Vector _R, Color _color, Color ret) {
	Vector N,R;
	copyVector(_N, N);
	copyVector(_R, R);
	normal(N, N);
	normal(R, R);
	Color color, tmp;

	copyColor(_color, color);
	mulc(color, BG_C, tmp);
	mulcd(tmp, DIFF, ret);

	float RN = dot(R, N);
	if (RN > EPS) {

		if (DIFF > EPS) {
			float diff = DIFF * RN;
			mulcd(color, diff, tmp);
			addc(ret, tmp, ret);
		}
		if (SPEC > EPS) {
		float spec = SPEC * pow(RN, SPEC_POWER);
		mulcd(color, spec, tmp);
		addc(ret, tmp, ret);
		}
	}
	normalize(ret);
}


bool CalcWeight(int i, int j, Pos v1, Pos v2, Pos v3, float k[], float *a, float *b, float *c)
{
	*a = ((v2[1] - v3[1]) * i + (v3[0] - v2[0]) * j + v2[0] * v3[1] - v3[0] * v2[1]) * k[1];
	*b = ((v3[1] - v1[1]) * i + (v1[0] - v3[0]) * j + v3[0] * v1[1] - v1[0] * v3[1]) * k[2];
	*c = 1 - *a - *b;
	if (*a >= 0 && *b >= 0 && *c >= 0) {
		return true;
	}
	else {
		return false;
	}
}


void drawTriangle(Pos v1, Pos v2, Pos v3, Color color) {
	float k[3];
	k[0] = ((v1[1] - v2[1]) * v3[0] + (v2[0] - v1[0]) * v3[1] + v1[0] * v2[1] - v2[0] * v1[1]);
	k[1] = ((v2[1] - v3[1]) * v1[0] + (v3[0] - v2[0]) * v1[1] + v2[0] * v3[1] - v3[0] * v2[1]);
	k[2] = ((v3[1] - v1[1]) * v2[0] + (v1[0] - v3[0]) * v2[1] + v3[0] * v1[1] - v1[0] * v3[1]);
	if (fabs(k[0]) < EPS) return;
	if (fabs(k[1]) < EPS) return;
	if (fabs(k[2]) < EPS) return;
	k[0] = 1 / k[0];
	k[1] = 1 / k[1];
	k[2] = 1 / k[2];
	int xMIN, xMAX, yMIN, yMAX;
	xMIN = round((v1[0] < v2[0] ? v1[0] : v2[0]) < v3[0] ? (v1[0] < v2[0] ? v1[0] : v2[0]) : v3[0]);
	xMAX = round((v1[0] > v2[0] ? v1[0] : v2[0]) > v3[0] ? (v1[0] > v2[0] ? v1[0] : v2[0]) : v3[0]);
	yMIN = round((v1[1] < v2[1] ? v1[1] : v2[1]) < v3[1] ? (v1[1] < v2[1] ? v1[1] : v2[1]) : v3[1]);
	yMAX = round((v1[1] > v2[1] ? v1[1] : v2[1]) > v3[1] ? (v1[1] > v2[1] ? v1[1] : v2[1]) : v3[1]);

	float zNowBuffer;
	float a = 0.333f, b = 0.333f, c = 0.333f;
	int i, j;
	for (i = xMIN; i <= xMAX; ++i) {
		for (j = yMIN; j <= yMAX; ++j) {
			if (CalcWeight(i, j, v1, v2, v3, k, &a, &b, &c)) {
				//zNowBuffer = a * v1[2] + b * v2[2] + c * v3[2];
				//setColorXY(i, j, color);
				bufferColor(i, j, color, (a * v1[2] + b * v2[2] + c * v3[2]));
			}
		}
		if (offline_render_status==RENDER_IDLE) return;
	}
}
bool getPos_online(Vector A,Pos B) {
	B[0]=PIC_W-(int)round(focus_x_r * A[0] / A[2] + center_x_r);
	B[1]=PIC_H-(int)round(focus_y_r * A[1] / A[2] + center_y_r);
	B[2]=A[2];
	return 0<=B[0] && B[0]<=PIC_W && 0<=B[1] && B[1]<=PIC_H;
}

void renderTriangle(Vector A, Vector B, Vector C, Color color) {
	Pos X, Y, Z;
	if (!getPos_online(A, X)) return;
	if (!getPos_online(B, Y)) return;
	if (!getPos_online(C, Z)) return;
	//if (!getPos(A, X)) return;
	//if (!getPos(B, Y)) return;
	//if (!getPos(C, Z)) return;
	drawTriangle(X, Y, Z, color);
}
void renderRect(Vector A, Vector B, Vector C, Vector D, Color color) {
	if (offline_render_status==RENDER_IDLE) return;
	renderTriangle(A, B, C, color);
	if (offline_render_status==RENDER_IDLE) return;
	renderTriangle(A, C, D, color);
	if (offline_render_status==RENDER_IDLE) return;
}

bool checkInCavans(Vector P[]){
	Pos X;
	for (int i=0;i<8;i++) if (!getPos_online(P[i],X)) return false;
	return true;
}

bool renderBox(Vector O, Vector X, Vector Y, Vector Z, Color color) {
	Vector P[8];
	Color col;
	copyColor(color, col);
	copyVector(O, P[0]);
	add(O, X, P[1]);
	add(P[1], Z, P[2]);
	add(O, Z, P[3]);
	for (int i = 0; i < 4; i++) {
		add(P[i], Y, P[i + 4]);
	}
	
	if (!checkInCavans(P)) return false;
	
	Vector nX, nY, nZ;
	copyNegVector(X, nX);
	copyNegVector(Y, nY);
	copyNegVector(Z, nZ);

	Vector tmp;
	muld(X, 0.5, tmp);
	add(O, tmp, O);
	muld(Y, 0.5, tmp);
	add(O, tmp, O);
	muld(Z, 0.5, tmp);
	add(O, tmp, O);

	Vector R={-5,-5,-5};
	Vector eye={0,0,100};

	float k=1;

	if (dot(eye,nX)<0){
		getColorNR(nX, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0], P[4], P[7], P[3], col);
	}
	printf("0: %d %d\n",dot(eye,nX)>0,(int)dot(eye,nX));
	//usleep(1000*2000);

	if (dot(eye,Z)<0){
		getColorNR(Z, R, color, col);//z
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[2], P[3], P[7], P[6], col);
	}
	printf("1: %d %d\n",dot(eye,Z)>0,(int)dot(eye,Z));
	//usleep(1000*2000);

	if (dot(eye,Y)<0){
		getColorNR(Y, R, color, col);//y
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[4], P[5], P[6], P[7], col);
	}
	printf("2: %d %d\n",dot(eye,Y)>0,(int)dot(eye,Y));
	//usleep(1000*2000);

	if (dot(eye,X)<0){
		getColorNR(X, R, color, col);//nx
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[1], P[2], P[6], P[5], col);
	}
	printf("3: %d %d\n",dot(eye,X)>0,(int)dot(eye,X));
	//usleep(1000*2000);
	
	if (dot(eye,nY)<0){
		getColorNR(nY, R, color, col);//ny
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0], P[1], P[2], P[3], col);
	}
	printf("4: %d %d\n",dot(eye,nY)>0,(int)dot(eye,nY));
	//usleep(1000*2000);


	if (dot(eye,nZ)<0){
		getColorNR(nZ, R, color, col);//nz
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0], P[1], P[5], P[4], col);
	}
	printf("5: %d %d\n",dot(eye,nZ)>0,(int)dot(eye,nZ));
	//usleep(1000*2000);
	
	return true;
}


void drawSphereLine(Pos2 p, int r, int x1, int x2, int y, Color color) {
	int j = y;
	int r2 = r*r;
	Vector R = { 1,0.7,0.8 };
	Vector col;
	for (int i = x1; i <= x2; i++) {
		int a2 = (i - p[0])*(i - p[0]) + (j - p[1])*(j - p[1]);
		float b = sqrt(abs(r2 - a2));
		Vector N = { i - p[0],j - p[1],b };
		//printf("%d %d\n", a2, r2);
		//debugVector(N);
		normal(N, N);
		N[2] += 0.5;
		normal(N, N);
		getColorNR(N, R, color, col);
		//setColorXY(i, j, col);
		bufferColor(i,j,col,-b);
		//printf("%d %d\n", i, j);
	}
}
//void drawSphere(Pos P, int r,Color color) {
//	int p0 = round(P[0]);
//	int p1 = round(P[1]);
//	int xMi = p0 - r;
//	if (xMi < 0) return;
//	int xMa = p0 + r;
//	if (xMa >= PIC_W) return;
//	int yMi = p1 - r;
//	if (yMi < 0) return;
//	int yMa = p1 + r;
//	if (yMa >= PIC_H) return;
//	int r2 = r*r;
//	Vector R = { 1,0.7,0.8 };
//	Color col;
//	for (int i = xMi; i <= xMa; i++) {
//		for (int j = yMi; j <= yMa; j++) {
//			int a2 = (i - p0)*(i - p0) + (j - p1)*(j - p1);
//			if (a2<=r2) {
//				float b = sqrt(r2 - a2);
//				Vector N = { i - p0,j - p1,b };
//				normal(N,N);
//				N[2] += 0.5;
//				normal(N, N);
//				getColorNR(N, R, color, col);
//				//printf("n  : "); debugVector(N);
//				//printf("col: "); debugVector(color);
//				setColorXY(i, j, col);
//			}
//		}
//	}
//}

void drawSphereC(Pos2 c, int radius, Color color) {
	int cx = c[0], cy = c[1];
	int dx = -1, dy = radius;
	float d = 1.25f - radius;
	while (dx <= dy) {
		if (d < 0) d += 2 * dx + 3;
		else d += 2 * (dx - dy) + 5, --dy;
		++dx;
		drawSphereLine(c, radius, -dx + cx, dx + cx, dy + cy, color);
		if (offline_render_status==RENDER_IDLE) return;
		drawSphereLine(c, radius, -dy + cx, dy + cx, dx + cy, color);
		if (offline_render_status==RENDER_IDLE) return;
		drawSphereLine(c, radius, -dy + cx, dy + cx, -dx + cy, color);
		if (offline_render_status==RENDER_IDLE) return;
		drawSphereLine(c, radius, -dx + cx, dx + cx, -dy + cy, color);
		if (offline_render_status==RENDER_IDLE) return;
	}
}
void renderSphere(Vector O, double R, Color color){
	Pos P, P1; Vector V;
	if (!getPos(O, P)) return;
	copyVector(O,V);
	V[1] += R;
	if (!getPos(V, P1)) return;
	int r = abs(P1[1] - P[1]);
	//drawSphere(P, r,color);
	Pos2 p = { P[0],P[1] };
	drawSphereC(p, r, color);
}


#endif