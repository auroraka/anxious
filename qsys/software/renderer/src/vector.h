#ifndef VECTOR3_H
#define VECTOR3_H

#include "basic.h"
#include <math.h>

typedef float Vector[3];

#define makeVector(x,y,z,C) (C[0]=x,C[1]=y,C[2]=z)
//void makeVector(float x, float y, float z, Vector C) {
//	C[0] = x;
//	C[1] = y;
//	C[2] = z;
//}
#define copyVector(A,C) (C[0]=A[0],C[1]=A[1],C[2]=A[2])
//void copyVector(const Vector A, Vector C) {
//	C[0] = A[0];
//	C[1] = A[1];
//	C[2] = A[2];
//}

#define add(A,B,C) 	(C[0] = A[0] + B[0],C[1] = A[1] + B[1],C[2] = A[2] + B[2])
//
//void add(const Vector A, const Vector B,Vector C) {
//	C[0] = A[0] + B[0];
//	C[1] = A[1] + B[1];
//	C[2] = A[2] + B[2];
//}

#define sub(A,B,C) 	(C[0] = A[0] - B[0],C[1] = A[1] - B[1],C[2] = A[2] - B[2])
//
//void sub(const Vector A, const Vector B,Vector C) {
//	C[0] = A[0] - B[0];
//	C[1] = A[1] - B[1];
//	C[2] = A[2] - B[2];
//}

#define muld(A,k,C) 	(C[0] = A[0] * k,C[1] = A[1] * k,C[2] = A[2] * k)
//
//void muld(const Vector A, float k,Vector C) {
//	C[0] = A[0] * k;
//	C[1] = A[1] * k;
//	C[2] = A[2] * k;
//}

#define divd(A,k,C)	(C[0] = A[0] / k,C[1] = A[1] / k,C[2] = A[2] / k)
//
//void divd(const Vector A, float k,Vector C) {
//	C[0] = A[0] / k;
//	C[1] = A[1] / k;
//	C[2] = A[2] / k;
//}

#define mul(A,B,C) 	(C[0] = A[1] * B[2] - A[2] * B[1],C[1] = A[2] * B[0] - A[0] * B[2],C[2] = A[0] * B[1] - A[1] * B[0])
//
//void mul(const Vector A, const Vector B,Vector C) {
//	C[0] = A[1] * B[2] - A[2] * B[1];
//	C[1] = A[2] * B[0] - A[0] * B[2];
//	C[2] = A[0] * B[1] - A[1] * B[0];
//}

#define neg(C) 	(C[0] = -C[0],C[1] = -C[1],C[2] = -C[2])

//void neg(Vector C) {
//	C[0] = -C[0];
//	C[1] = -C[1];
//	C[2] = -C[2];
//}

#define dot(A,B) (A[0] * B[0] + A[1] * B[1] + A[2] * B[2])
//float dot(Vector A, const Vector B) {
//	return A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
//}

#define len2(A) (A[0] * A[0] + A[1] * A[1] + A[2] * A[2])
//float len2(Vector A) {
//	return A[0] * A[0] + A[1] * A[1] + A[2] * A[2];
//}

#define len(A) (sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]))
//float len(Vector A) {
//	return sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
//}

#define dis2(A,B) ((A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]) + (A[2] - B[2])*(A[2] - B[2]))
//float dis2(Vector A,Vector B) {
//	return (A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]) + (A[2] - B[2])*(A[2] - B[2]);
//}

#define dis(A,B) (sqrt((A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]) + (A[2] - B[2])*(A[2] - B[2])))
//float dis(Vector A,Vector B) {
//	return sqrt((A[0] - B[0])*(A[0] - B[0]) + (A[1] - B[1])*(A[1] - B[1]) + (A[2] - B[2])*(A[2] - B[2]));
//}


void normal(Vector A,Vector C) {
	float l = len(A);
	C[0] = A[0] / l;
	C[1] = A[1] / l;
	C[2] = A[2] / l;
}

#define isZero(A) (fabs(A[0]) < EPS && fabs(A[1]) < EPS && fabs(A[2]) < EPS)
//int isZero(Vector A) {
//	return fabs(A[0]) < EPS && fabs(A[1]) < EPS && fabs(A[2]) < EPS;
//}
void GetAnVerticalVector(Vector A,Vector ret) {
	Vector tmp;
	makeVector(0, 0, 1, tmp);
	mul(A, tmp, ret);
	if (isZero(ret)) makeVector(1, 0, 0, ret);
	else normal(ret,ret);
}



#endif
