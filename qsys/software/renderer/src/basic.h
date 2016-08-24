#ifndef BASIC_H
#define BASIC_H


#define PIC_H (480)
#define PIC_W (640)

typedef unsigned char uchar;

const float SPEC_POWER = 4;

const static float EPS = 1e-6;
const static float INF = 1e30;
const static float PI = 3.1415926536897931;

inline float sqr(float a) {
	return a*a;
}



#endif 