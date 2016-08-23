#ifndef COLOR_H
#define COLOR_H

#include "basic.h"


typedef float Color[3];

#define makeColor(r,g,b,C) (		C[0] = r,C[1] = g,C[2] = b)
	//void makeColor(float r, float g, float b,Color C) {
	//	C[0] = r;
	//	C[1] = g;
	//	C[2] = b;
	//}
	void normalize(Color C) {
		if (C[0] > 1) C[0] = 1;
		if (C[1] > 1) C[1] = 1;
		if (C[2] > 1) C[2] = 1;
	}
#define addc(A,B,C) (C[0] = A[0] + B[0],C[1] = A[1] + B[1],C[2] = A[2] + B[2])

	/*void addc( const Color A , const Color B ,Color C) {
		C[0] = A[0] + B[0];
		C[1] = A[1] + B[1];
		C[2] = A[2] + B[2];
	}
	*/
#define subc(A,B,C) (C[0] = A[0] - B[0],C[1] = A[1] - B[1],C[2] = A[2] - B[2])

	//void subc( const Color A , const Color B ,Color C) {
	//	C[0] = A[0] - B[0];
	//	C[1] = A[1] - B[1];
	//	C[2] = A[2] - B[2];
	//}

#define mulc(A,B,C) (		C[0] = A[0] * B[0],C[1] = A[1] * B[1],C[2] = A[2] * B[2])
	//void mulc( const Color A , const Color B ,Color C) {
	//	C[0] = A[0] * B[0];
	//	C[1] = A[1] * B[1];
	//	C[2] = A[2] * B[2];
	//}
	
#define mulcd(A,k,C) (		C[0] = A[0] * k,C[1] = A[1] * k,C[2] = A[2] * k)
	//void mulcd( const Color A , float k ,Color C) {
	//	C[0] = A[0] * k;
	//	C[1] = A[1] * k;
	//	C[2] = A[2] * k;
	//}

#define divcd(A,k,C) (C[0] = A[0] / k,C[1] = A[1] / k,C[2] = A[2] / k)
	//void divcd( const Color A , float k ,Color C) {
	//	C[0] = A[0] / k;
	//	C[1] = A[1] / k;
	//	C[2] = A[2] / k;
	//}
#define copyColor(A,C) (		C[0] = A[0],C[1] = A[1],C[2] = A[2])
	//void copyColor(const Color A, Color C) {
	//	C[0] = A[0];
	//	C[1] = A[1];
	//	C[2] = A[2];
	//}
	
#endif