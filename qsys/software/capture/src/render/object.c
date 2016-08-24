//
// Created by Kanari on 2016/8/24.
//

#include "object.h"
#include "../memory.h"

static volatile RawSphere *raw_objects;

void reset_objects() {
	OBJECT_CNT_W(0);
	raw_objects = (volatile RawSphere *)(SHARED_MEMORY + 6);
}

void remove_object(int idx) {
	int i, cnt = OBJECT_CNT_R();
	for (i = idx + 1; i < cnt; ++i)
		raw_objects[i] = raw_objects[i + 1];
	OBJECT_CNT_W(cnt - 1);
}

void add_sphere(float x, float y, float z, float radius, unsigned color) {
	int cnt = OBJECT_CNT_R();
	raw_objects[cnt].x = x;
	raw_objects[cnt].y = y;
	raw_objects[cnt].z = z;
	raw_objects[cnt].radius = radius;
	raw_objects[cnt].color = color;
	OBJECT_CNT_W(cnt + 1);
}

volatile RawSphere get_sphere(int idx) {
	return raw_objects[idx];
}
