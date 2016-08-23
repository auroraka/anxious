#ifndef BASIC_H
#define BASIC_H

typedef unsigned char uchar;

const float SPEC_POWER = 20;
const int MAX_RAYTRACING_DEP = 1;

const static float EPS = 1e-6;
const static float INF = 1e30;
const static float PI = 3.1415926536897931f;

inline float sqr(float a) {
	return a * a;
}

#endif