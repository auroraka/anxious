#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "basic.h"
#include "vector.h"
#include "pos.h"

typedef float Mat[16];
#define At(m,i,j) m[(j)*4 +(i)]

static Mat T;

void clearMat(Mat m) {
	memset(m, 0, sizeof(float) * 16);
}
void makeOne(Mat m) {
	clearMat(m);
	At(m, 0, 0) = 1;
	At(m, 1, 1) = 1;
	At(m, 2, 2) = 1;
	At(m, 3, 3) = 1;
}
void copyMat(Mat a, Mat b) {
	memcpy(b, a, sizeof(float) * 16);
}
void mulm(Mat A, Mat B, Mat C) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			At(C, i, j) = 0;
			for (int k = 0; k < 4; k++) {
				At(C, i, j) += At(A, i, k)*At(B, k, j);
			}
		}
	}
}
void mulmv(Mat m, Vector A, Vector B) {
	float v[4];
	for (int i = 0; i < 4; i++) {
		v[i] = At(m, i, 0)*A[0] + At(m, i, 1)*A[1] + At(m, i, 2)*A[2] + At(m, i, 3);
	}
	B[0] = v[0] / v[3];
	B[1] = v[1] / v[3];
	B[2] = v[2] / v[3];
}
void getViewPortTranform(float N,float F,Mat m) {
	float  left = -1,right = 1;
	float button = -1, top = 1;
	clearMat(m);
	At(m, 0, 0) = 2 * N / (right - left);
	At(m, 0, 2) = (right + left) / (right - left);
	At(m, 1, 1 ) = 2 * N / (top - button);
	At(m, 1, 2) = (top + button) / (top - button);
	At(m, 2, 2) = -(F + N) / (F - N);
	At(m, 2, 3) = -2 * F*N / (F - N);
	At(m, 3, 2) = 1;//?
}
void getCamFrontTransform(Vector eye, Vector front, Vector up, Mat m) {
	Vector x, y, z, upV;
	Mat A, B;

	copyVector(front, z);
	normal(z, z);

	copyVector(up, upV);
	normal(upV, upV);
	cross(z, upV, x);
	normal(x, x);
	cross(x, z, y);
	clearMat(A);
	for (int i = 0; i < 3; i++) {
		At(A, i, 0) = -x[i];//?
		At(A, i, 1) = -y[i];//?
		At(A, i, 2) = z[i];
	}
	At(A, 3, 3) = 1;

	makeOne(B);
	At(B, 0, 3) = -eye[0];
	At(B, 1, 3) = -eye[1];
	At(B, 2, 3) = -eye[2];
	mulm(A, B, m);
	//copyMat(B, m);
}
void getCamLookAtTransform(Vector eye,Vector lookAt,Vector up,Mat m) {
	Vector z;
	sub(lookAt, eye, z);
	getCamFrontTransform(eye,z,up,m);
}

void initTransform() {
	Mat A,B;
	Vector eye = { 0,0,0 };
	Vector up = { 0,1,0 };
	//Vector lookAt = { 0,0,100 };
	Vector front = { 0,0,-1 };
	//getCamLookAtTransform(eye,lookAt,up,A);
	getCamFrontTransform(eye, front, up, A);
	getViewPortTranform(1, 500, B);
	mulm(B, A ,T);
	//copyMat(B, T);
}
bool getPos(Vector A,Pos X) {
	mulmv(T, A, X);
	//debugVector(A);
	//debugVector(X);
	X[0] = X[0]*PIC_W+0.5f*PIC_W;
	X[1] = X[1]*PIC_W+0.5f*PIC_H;
	//debugVector(X);
	//printf("#\n");
	return (0 <= X[0] && X[0] <= PIC_W && 0 <= X[1] && X[1] <= PIC_H);
}
#endif