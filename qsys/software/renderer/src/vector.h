#ifndef VECTOR3_H
#define VECTOR3_H

#include "basic.h"
#include <math.h>

//#define INT_VECTOR

#ifdef INT_VECTOR
typedef int Vector[3];
#else
typedef float Vector[3];
#endif

#define makeVector(x,y,z,C) (C[0]=x,C[1]=y,C[2]=z)

#define copyVector(A,C) (C[0]=A[0],C[1]=A[1],C[2]=A[2])

#define add(A,B,C) 	(C[0] = A[0] + B[0],C[1] = A[1] + B[1],C[2] = A[2] + B[2])

#define sub(A,B,C) 	(C[0] = A[0] - B[0],C[1] = A[1] - B[1],C[2] = A[2] - B[2])

#define muld(A,k,C) 	(C[0] = A[0] * k,C[1] = A[1] * k,C[2] = A[2] * k)

#define neg(C) 	(C[0] = -C[0],C[1] = -C[1],C[2] = -C[2])

#define dot(A,B) (A[0] * B[0] + A[1] * B[1] + A[2] * B[2])

#define len2(A) (A[0] * A[0] + A[1] * A[1] + A[2] * A[2])

#define len(A) (sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]))

void normal(Vector A,Vector C) {
	float l = len(A);
	C[0] = A[0] / l;
	C[1] = A[1] / l;
	C[2] = A[2] / l;
}

#ifdef INT_VECTOR
#define isZero(A) (A[0]==0 && A[1]==0 && A[2]==0)
#else
#define isZero(A) (fabs(A[0]) < EPS && fabs(A[1]) < EPS && fabs(A[2]) < EPS)
#endif

#endif
