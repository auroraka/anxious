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

//SPEC_POWER=4
inline float powSpec(float x){
	return x*x*x*x;
}
//notice: N,R,color will change after function
void getColorNR(Vector N, Vector R, Color _color, Color ret) {
	Color color, tmp;

	copyColor(_color, color);
	mulc(color, BG_C, tmp);
	mulcd(tmp, DIFF, ret);

	float RN = dot(R, N);
	if (RN > EPS) {
		//DIFF
		float diff = DIFF * RN;
		mulcd(color, diff, tmp);
		addc(ret, tmp, ret);
		
		//SPEC
		//float spec = SPEC * pow(RN, SPEC_POWER);
		float spec = SPEC * powSpec(RN);
		mulcd(color, spec, tmp);
		addc(ret, tmp, ret);
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
		if (RENDER_STATUS_R()==RENDER_IDLE) return;
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
	getPos_online(A, X);
	getPos_online(B, Y);
	getPos_online(C, Z);
	//if (!getPos_online(A, X)) return;
	//if (!getPos_online(B, Y)) return;
	//if (!getPos_online(C, Z)) return;
	//if (!getPos(A, X)) return;
	//if (!getPos(B, Y)) return;
	//if (!getPos(C, Z)) return;
	drawTriangle(X, Y, Z, color);
}
void renderRect(Vector A, Vector B, Vector C, Vector D, Color color) {
	if (RENDER_STATUS_R()==RENDER_IDLE) return;
	renderTriangle(A, B, C, color);
	renderTriangle(A, C, D, color);
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
	
	normal(X,X);normal(Y,Y);normal(Z,Z);
	Vector nX, nY, nZ;
	copyNegVector(X, nX);
	copyNegVector(Y, nY);
	copyNegVector(Z, nZ);

	Vector R={-5,-5,-5};
	normal(R,R);
	Vector eye={0,0,100};

	float k=1;

	if (dot(eye,nX)<0){
		getColorNR(nX, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0], P[4], P[7], P[3], col);
	}
	if (dot(eye,Z)<0){
		getColorNR(Z, R, color, col);//z
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[2], P[3], P[7], P[6], col);
	}
	if (dot(eye,Y)<0){
		getColorNR(Y, R, color, col);//y
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[4], P[5], P[6], P[7], col);
	}
	if (dot(eye,X)<0){
		getColorNR(X, R, color, col);//nx
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[1], P[2], P[6], P[5], col);
	}
	if (dot(eye,nY)<0){
		getColorNR(nY, R, color, col);//ny
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0], P[1], P[2], P[3], col);
	}
	if (dot(eye,nZ)<0){
		getColorNR(nZ, R, color, col);//nz
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0], P[1], P[5], P[4], col);
	}
	return true;
}

bool renderPyramid(Vector O, Vector X, Vector Z, Vector A, Color color) {
	Vector P[4];
	Color col;
	copyColor(color, col);
	copyVector(O, P[0]);
	add(O, X, P[1]);
	add(P[1], Z, P[2]);
	add(O, Z, P[3]);
	
	Vector R={-5,-5,-5};
	normal(R,R);
	Vector eye={0,0,100};

	float k=1;

	normal(X,X);normal(Z,Z);
	Vector nX, nZ;
	copyNegVector(X, nX);
	copyNegVector(Z, nZ);
	
	if (dot(eye,nZ)){
		getColorNR(nZ, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderTriangle(P[0],P[1],A,col);
	}

	if (dot(eye,X)){
		getColorNR(X, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderTriangle(P[1],P[2],A,col);
	}
	
	if (dot(eye,Z)){
		getColorNR(Z, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderTriangle(P[2],P[3],A,col);
	}
	
	if (dot(eye,nX)){
		getColorNR(nX, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderTriangle(P[3],P[0],A,col);
	}
	Vector N;
	cross(Z,X,N);
	normal(N,N);
	if (dot(eye,N)<0){
		getColorNR(N, R, color, col);//x
		mulcd(col, k, col);
		normalize(col);
		renderRect(P[0],P[1],P[2],P[3],col);
	}
	return true;
}


int sqrti(int a){
	if (a<=1) return 1;
	int l=1,r=a,mid;
	while (l<r){
		mid=(l+r)>>1;
		if ((mid+1)*(mid+1)>a) r=mid;
		else l=mid+1;
	}
	return l;
}
void drawSphereLine(Pos2 p, int r, int x1, int x2, int y, Color color) {
	int j = y;
	int r2 = r*r;
	Vector R = { 1,0.7,0.8 };
	Vector col;
	for (int i = x1; i <= x2; i++) {
		int a2 = (i - p[0])*(i - p[0]) + (j - p[1])*(j - p[1]);
		float b = sqrti(abs(r2 - a2));
		Vector N = { i - p[0],j - p[1],b };
		//sprintf(MSG,"%d %d\n", a2, r2);
		//debugVector(N);
		normal(N, N);
		N[2] += 0.5;
		normal(N, N);
		getColorNR(N, R, color, col);
		//setColorXY(i, j, col);
		bufferColor(i,j,col,-b);
		//sprintf(MSG,"%d %d\n", i, j);
	}
}
void drawSphereLine3d(Vector O, float r3d, Pos p, int r, int x1, int x2, int y, Color color) {
  // Calculation of distance
  // solve for P(x, y, z) whose projection on the plane is (i, j)
  //   AND on the surface of the sphere
  // P = P_z * (r_x, r_y, 1), where
  //   r_x = (PIC_W - center_x - i) / focus_x
  //   r_y = (PIC_H - center_y - j) / focus_y
  // => (r_x, r_y, 1)^2 P_z^2 - 2 P_z (r_x, r_y, 1) O + O^2 - r^2 = 0

	int j = y;
	int r2 = r*r;
	Vector R = {0.6851887,0.4796320,0.5481509};//normalized //{ 1,0.7,0.8 };
	Vector col;

	for (int i = x1; i <= x2; i++) {
		int a2 = (i - p[0])*(i - p[0]) + (j - p[1])*(j - p[1]);
		float b = sqrt(abs(r2 - a2));
		Vector N = { i - p[0],j - p[1],b };
		normal(N, N);
		getColorNR(N, R, color, col);
    float r_x = (PIC_W - center_x_r - i) / focus_x_r;
    float r_y = (PIC_H - center_y_r - j) / focus_y_r;
    // coefficients of the quadratic equation
    float q_A = r_x * r_x + r_y * r_y + 1;
    float q_B = 2 * (r_x * O[0] + r_y * O[1] + O[2]);
    float q_C = len2(O) - r2;
    float q_delta = q_B * q_B - 4 * q_A * q_C;
    if (q_delta > EPS) {
      float dis = (-q_B - sqrt(q_delta)) / (2 * q_A);
      bufferColor(i, j, col, dis);
    }
	}
}


void drawSphereC(Pos2 c, int radius, Color color) {
	int cx = c[0], cy = c[1];
	int dx = -1, dy = radius;
	float d = 1.25f - radius;
	while (dx <= dy) {
		if (d < 0) d += 2 * dx + 3;
		else d += 2 * (dx - dy) + 5, --dy;
		++dx;
		drawSphereLine(c, radius, -dx + cx, dx + cx, dy + cy, color);
		drawSphereLine(c, radius, -dy + cx, dy + cx, dx + cy, color);
		drawSphereLine(c, radius, -dy + cx, dy + cx, -dx + cy, color);
		drawSphereLine(c, radius, -dx + cx, dx + cx, -dy + cy, color);
		if (RENDER_STATUS_R()==RENDER_IDLE) return;
	}
}
void drawSphere3d(Vector O, float r3d, Pos c, int radius, Color color) {
	int cx = c[0], cy = c[1];
	int dx = -1, dy = radius;
	float d = 1.25f - radius;
	while (dx <= dy) {
		if (d < 0) d += 2 * dx + 3;
		else d += 2 * (dx - dy) + 5, --dy;
		++dx;
		drawSphereLine3d(O, r3d, c, radius, -dx + cx, dx + cx, dy + cy, color);
		drawSphereLine3d(O, r3d, c, radius, -dy + cx, dy + cx, dx + cy, color);
		drawSphereLine3d(O, r3d, c, radius, -dy + cx, dy + cx, -dx + cy, color);
		drawSphereLine3d(O, r3d, c, radius, -dx + cx, dx + cx, -dy + cy, color);
		if (RENDER_STATUS_R()==RENDER_IDLE) return;
	}
}
void drawSphere3dHalf(Pos c, int radius,float k, Color color) {
	int cx = c[0], cy = c[1];
	int dx = -1, dy = radius;
	float d = 1.25f - radius;
	while (dx <= dy) {
		if (d < 0) d += 2 * dx + 3;
		else d += 2 * (dx - dy) + 5, --dy;
		++dx;
		#if CPU_ID==3
		drawSphereLine3d(c, radius,k, -dx + cx, dx + cx, dy + cy, color);
		drawSphereLine3d(c, radius,k, -dy + cx, dy + cx, dx + cy, color);
		#else
		drawSphereLine3d(c, radius, k,-dy + cx, dy + cx, -dx + cy, color);
		drawSphereLine3d(c, radius, k,-dx + cx, dx + cx, -dy + cy, color);
		#endif
		if (RENDER_STATUS_R()==RENDER_IDLE) return;
	}
}
bool renderSphere3d(Vector O, Vector X, Color color){
	Pos P,P1;
	getPos_online(O,P);
	getPos_online(X,P1);
	//if (!getPos_online(O,P)) return false;
	//if (!getPos_online(X,P1)) return false;
	
	float r2d=sqrt((P[0]-P1[0])*(P[0]-P1[0])+(P[1]-P1[1])*(P[1]-P1[1]));
	if (r2d<EPS) return false;
	float r3d=dis(O,X);
	drawSphere3d(O, r3d, P, r2d, color);
	return true;
	
}
bool renderSphere3dHalf(Vector O, Vector X, Color color){
	Pos P,P1;
	getPos_online(O,P);
	getPos_online(X,P1);
	//if (!getPos_online(O,P)) return false;
	//if (!getPos_online(X,P1)) return false;
	
	float r2d=sqrt((P[0]-P1[0])*(P[0]-P1[0])+(P[1]-P1[1])*(P[1]-P1[1]));
	if (r2d<EPS) return false;
	float r3d=dis(O,X);
	float k=r3d/r2d;
	drawSphere3dHalf(P, r2d, k,color);
	return true;
	
}


#endif