#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "color.h"
#include "vector.h"
#include "basic.h"
#include "../common.h"

struct IntersectResult {
	Vector N, C;
	float dist;
	int front;
};

typedef struct {
	float x, y, z, radius;
	unsigned color;
} RawSphere;

#define OBJECT_CNT_R() SHARED_R(5)
#define OBJECT_CNT_W(x) SHARED_W(5, x)

void reset_objects();

void remove_object(int idx);

void add_sphere(float x, float y, float z, float radius, unsigned color);

volatile RawSphere get_sphere(int idx);

#endif
