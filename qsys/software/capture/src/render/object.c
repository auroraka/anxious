//
// Created by Kanari on 2016/8/24.
//

#include <string.h>

#include "object.h"
#include "../memory.h"

static volatile RawObject *raw_objects = (volatile RawObject *)((unsigned *)SHARED_MEMORY_BASE + 6);

void reset_objects() {
	OBJECT_CNT_W(0);
}

void remove_object(int idx) {
	int i, cnt = OBJECT_CNT_R();
	for (i = idx + 1; i < cnt; ++i)
		raw_objects[i] = raw_objects[i + 1];
	OBJECT_CNT_W(cnt - 1);
}

void add_sphere(pointf *center, float radius, unsigned color) {
	int cnt = OBJECT_CNT_R();
	printf("Sphere: (%d,%d,%d) radius=%d\n", (int)center->x, (int)center->y, (int)center->z, (int)radius);
	raw_objects[cnt].p[0].x = -center->x;
	raw_objects[cnt].p[0].y = -center->y;
	raw_objects[cnt].p[0].z = center->z - 150;
	raw_objects[cnt].p[1].x = radius;
	raw_objects[cnt].color = color;
	OBJECT_CNT_W(cnt + 1);
}
void add_sphere2d(pointf *center, float radius, unsigned color) {
	int cnt = OBJECT_CNT_R();
	printf("Sphere: (%d,%d) radius=%d\n", (int)center->x, (int)center->y,(int)radius);
	raw_objects[cnt].p[0].x = center->x;
	raw_objects[cnt].p[0].y = center->y;
	raw_objects[cnt].p[1].x = radius;
	raw_objects[cnt].color = color;
	OBJECT_CNT_W(cnt + 1);
}

void add_cube(pointf pf[], unsigned color) {
	int cnt = OBJECT_CNT_R();
	printf("Cube\n");
	memcpy(raw_objects[cnt].p, pf, sizeof(float) * 12);
	raw_objects[cnt].color = color;
	OBJECT_CNT_W(cnt + 1);
}

void get_sphere(int idx, RawSphere *sphere) {
	//memcpy(sphere, raw_objects[idx].p, sizeof(float) * 4);
	sphere->x=raw_objects[idx].p[0].x;
	sphere->y=raw_objects[idx].p[0].y;
	sphere->z=raw_objects[idx].p[0].z;
	sphere->radius=raw_objects[idx].p[1].x;
	sphere->color = raw_objects[idx].color;
}
