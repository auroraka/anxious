#ifndef ANXIOUS_RENDER_BASIC_H
#define ANXIOUS_RENDER_BASIC_H

typedef unsigned char uchar;

static const float SPEC_POWER = 4;

static const float EPS = 1e-6;
static const float INF = 1e30;
static const float PI = 3.1415926536897931f;

inline static float sqr(float a) {
	return a * a;
}

#endif