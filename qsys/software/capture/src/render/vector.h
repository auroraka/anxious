#ifndef ANXIOUS_RENDER_VECTOR_H
#define ANXIOUS_RENDER_VECTOR_H

#include "basic.h"
#include <math.h>

typedef float Vector[3];

#define makeVector(x, y, z, C) (C[0] = x, C[1] = y, C[2] = z)
#define copyVector(A, C) (C[0] = A[0], C[1] = A[1], C[2] = A[2])
#define copyNegVector(A,C) (C[0]=-A[0],C[1]=-A[1],C[2]=-A[2])
#define add(A, B, C) (C[0] = A[0] + B[0], C[1] = A[1] + B[1], C[2] = A[2] + B[2])
#define sub(A, B, C) (C[0] = A[0] - B[0], C[1] = A[1] - B[1], C[2] = A[2] - B[2])
#define muld(A, k, C) (C[0] = A[0] * k, C[1] = A[1] * k, C[2] = A[2] * k)
#define divd(A, k, C) (C[0] = A[0] / k, C[1] = A[1] / k, C[2] = A[2] / k)
#define mul(A, B, C) (C[0] = A[1] * B[2] - A[2] * B[1], C[1] = A[2] * B[0] - A[0] * B[2], C[2] = A[0] * B[1] - A[1] * B[0])
#define neg(C) (C[0] = -C[0], C[1] = -C[1], C[2] = -C[2])
#define dot(A, B) (A[0] * B[0] + A[1] * B[1] + A[2] * B[2])
#define len2(A) (A[0] * A[0] + A[1] * A[1] + A[2] * A[2])
#define len(A) (sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]))
#define dis2(A, B) ((A[0] - B[0]) * (A[0] - B[0]) + (A[1] - B[1]) * (A[1] - B[1]) + (A[2] - B[2]) * (A[2] - B[2]))
#define dis(A, B) (sqrt((A[0] - B[0]) * (A[0] - B[0]) + (A[1] - B[1]) * (A[1] - B[1]) + (A[2] - B[2]) * (A[2] - B[2])))
#define cross(A,B,C) 	(C[0] = A[1] * B[2] - A[2] * B[1],C[1] = A[2] * B[0] - A[0] * B[2],C[2] = A[0] * B[1] - A[1] * B[0])


float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	// /y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

inline static void normal(Vector A, Vector C) {
	// float l = len(A);
	// C[0] = A[0] / l;
	// C[1] = A[1] / l;
	// C[2] = A[2] / l;
	float x=len2(A);
	float y=Q_rsqrt(x);
	C[0]=A[0]*y;
	C[1]=A[1]*y;
	C[2]=A[2]*y;
}

#define isZero(A) (fabs(A[0]) < EPS && fabs(A[1]) < EPS && fabs(A[2]) < EPS)

inline static void GetAnVerticalVector(Vector A, Vector ret) {
	Vector tmp;
	makeVector(0, 0, 1, tmp);
	mul(A, tmp, ret);
	if (isZero(ret)) makeVector(1, 0, 0, ret);
	else normal(ret, ret);
}


#endif
