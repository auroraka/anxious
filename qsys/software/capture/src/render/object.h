#ifndef ANXIOUS_RENDER_OBJECT_H
#define ANXIOUS_RENDER_OBJECT_H

#include "color.h"
#include "vector.h"
#include "basic.h"
#include "../common.h"
#include "../memory.h"

struct IntersectResult {
	Vector N, C;
	float dist;
	int front;
};

typedef struct {
	int x, y;
} pointi;

typedef struct {
	float x, y, z;
} pointf;

typedef struct {
	float x, y, z, radius;
	unsigned color;
} RawSphere;

typedef struct {
	pointf p[4];
	unsigned color;
} RawObject;

#define OBJECT_CNT_R() SHARED_R(666)
#define OBJECT_CNT_W(x) SHARED_W(666, x)

void reset_objects();

void remove_object(int idx);

void add_sphere(pointf *center, float radius, unsigned color);

void add_sphere2d(pointf *center, float radius, unsigned color);

void add_cube(pointf center[], unsigned color);

void get_sphere(int idx, RawSphere *sphere);

void delete_all();

static volatile RawObject *raw_objects = (volatile RawObject *)((unsigned *)SHARED_MEMORY_BASE + 6);

#endif
