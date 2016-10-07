#ifndef ANXIOUS_RENDER_OBJECT_H
#define ANXIOUS_RENDER_OBJECT_H

#include "../common.h"
#include "../memory.h"

#define RENDER_IDLE (0)
#define RENDER_CLEAR (1)
#define RENDER_ADD_SPHERE (2)
#define RENDER_ADD_SPHERE3D (3)
#define RENDER_ADD_CUBE (4)

#define RENDER_STATUS_R() SHARED_R(5)
#define RENDER_STATUS_W(x) SHARED_W(5, x)

#define OBJECT_CNT_R() SHARED_R(6)
#define OBJECT_CNT_W(x) SHARED_W(6, x)

#define OBJECT_LENGTH (15)
#define OBJECT_TOT (20)

#define OBJECT_R(i,j) SHARED_R(7+i*OBJECT_LENGTH+j)
#define OBJECT_W(i,j,x) SHARED_W(7+i*OBJECT_LENGTH+j,x)

typedef struct {
	int x, y;
} pointi;

typedef struct {
	float x, y, z;
} pointf;


void reset_objects();

void remove_object(int idx);

void add_sphere3d(pointf *pf, unsigned color);

void add_sphere2d(int x,int y,int r, unsigned color);

void add_cube(pointf center[], unsigned color);

void delete_all();

#endif
