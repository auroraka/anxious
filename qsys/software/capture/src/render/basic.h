#ifndef ANXIOUS_RENDER_BASIC_H
#define ANXIOUS_RENDER_BASIC_H


#define PIC_H (480)
#define PIC_W (640)

typedef unsigned char uchar;

#define SPEC_POWER (4)

const static float EPS = 1e-6;
const static float INF = 1e30;
const static float PI = 3.1415926536897931;

#define bool int
//#define true (1)
//#define false (0)

#define EMPTY_TYPE (0)
#define SPHERE_TYPE (1)
#define BOX_TYPE (2)
#define TRIANGLE_TYPE (3)

#define DIFF (0.6)
#define SPEC (0.4)

#endif