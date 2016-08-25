#ifndef COLOR_H
#define COLOR_H

#include "basic.h"

//#define INT_COLOR


#ifdef INT_COLOR
#define COLOR_MAX 255
typedef int Color[3];
#else
#define COLOR_MAX 1
typedef float Color[3];
#endif


#define makeColor(r,g,b,C) (		C[0] = r,C[1] = g,C[2] = b)

void normalize(Color C) {
	if (C[0] > COLOR_MAX) C[0] = COLOR_MAX;
	if (C[1] > COLOR_MAX) C[1] = COLOR_MAX;
	if (C[2] > COLOR_MAX) C[2] = COLOR_MAX;
}

#define addc(A,B,C) (C[0] = A[0] + B[0],C[1] = A[1] + B[1],C[2] = A[2] + B[2])

#define subc(A,B,C) (C[0] = A[0] - B[0],C[1] = A[1] - B[1],C[2] = A[2] - B[2])

#define mulcd(A,k,C) (		C[0] = A[0] * k,C[1] = A[1] * k,C[2] = A[2] * k)

#define copyColor(A,C) (		C[0] = A[0],C[1] = A[1],C[2] = A[2])

#endif