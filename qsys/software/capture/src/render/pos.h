#ifndef POS_H
#define POS_H

#include "basic.h"

typedef float Pos[3];


#define makePos(x,y,z,C) (C[0]=x,C[1]=y,C[2]=z)

#define copyPos(A,C) (C[0]=A[0],C[1]=A[1],C[2]=A[2])

#define copyNegPos(A,C) (C[0]=-A[0],C[1]=-A[1],C[2]=-A[2])

inline void swapPos(Pos A, Pos B) {
	int t;
	t = A[0]; A[0] = B[0]; B[0] = t;
	t = A[1]; A[1] = B[1]; B[1] = t;
	t = A[2], A[2] = B[2], B[2] = t;
}

void debugPos(Pos X) {
	sprintf(MSG,"%.5lf %.5lf %.5lf\n", X[0], X[1], X[2]);
	debugMSG();
}

typedef int Pos2[2];
#endif 