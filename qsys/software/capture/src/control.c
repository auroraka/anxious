//
// Created by Kanari on 2016/8/15.
//

#ifndef CPU_ID
#define CPU_ID 2
#endif

#if CPU_ID == 2

#include <unistd.h>

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "memory.h"
#include "common.h"
#include "display.h"
#include "control.h"
#include "render/object.h"

const float focus_x_l = 1117.36809f, focus_y_l = 1115.59155f;
const float focus_x_r = 1098.56402f, focus_y_r = 1095.74358f;
const float center_x_l = 284.432591f, center_y_l = 205.206010f;
const float center_x_r = 260.334008f, center_y_r = 307.355809f;
const float stereo_dist = 17.6; // cm

#define VSYNC(x) IOWR(OVERLAY_VSYNC_PIO_BASE, 0, (x))
#define BUFFER_PORT() IORD(OVERLAY_PORT_PIO_BASE, 0)
//#define BUFFER_PORT() IORD(RENDER_PORT_PIO_BASE, 0)

char msg[100], last_msg[100];

Location find_location(point p_l, point p_r) {
	int x_l = get_x(p_l), x_r = get_x(p_r);
	int y_l = get_y(p_l), y_r = get_y(p_r);
	
	Location loc;
	loc.z = stereo_dist / ((x_l - center_x_l) / focus_x_l - (x_r - center_x_r) / focus_x_r);
	loc.x = loc.z * (x_r - center_x_r) / focus_x_r;
	loc.y = loc.z * (get_y(p_r) - center_y_r) / focus_y_r;
	
	memcpy(last_msg, msg, sizeof(msg));
	sprintf(msg, "Left (%3d,%3d)\nRight (%3d,%3d)\nPosition (%3d,%3d,%3d)",
	        x_l, y_l, x_r, y_r, (int)loc.x, (int)loc.y, (int)loc.z);
	
	return loc;
}

pointi project_point(pointf p) {
	pointi ret;
	ret.x = (int)round(focus_x_r * p.x / p.z + center_x_r);
	ret.y = (int)round(focus_y_r * p.y / p.z + center_y_r);
	return ret;
}

enum DRAW_STATE {
	DRAW_POINT,
	DRAW_SPHERE_RADIUS,
	DRAW_CUBE_LINE,
	DRAW_CUBE_AREA,
	DRAW_CUBE_VOLUME
} draw_state;

//#define OVERLAY_W(x, y, val) (SDRAM[(2 << 23) | (overlay_port << 19) | ((y) << 10) | (x)] = (val))
//#define OVERLAY_W(x, y, val) (SDRAM[(1 << 23) | (overlay_port << 19) | ((y) << 10) | (x)] = (val))

unsigned overlay_port = 0;

inline void OVERLAY_W(int x, int y, unsigned val) {
	if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
		SDRAM[(2 << 23) | (overlay_port << 19) | ((y) << 10) | (WIDTH - x - 1)] = (val);
}

void draw_sphere(pointi c, float radius, unsigned color) {
	int cx = c.x, cy = c.y;
//void draw_sphere(int cx, int cy, float radius, unsigned color) {
	int dx = 0, dy = (int)radius;
	float d = 1.25f - radius;
	while (dx <= dy) {
		if (d < 0) d += 2 * dx + 3;
		else d += 2 * (dx - dy) + 5, --dy;
		++dx;
		
		OVERLAY_W( dx + cx,  dy + cy, color);
		OVERLAY_W( dy + cx,  dx + cy, color);
		OVERLAY_W(-dx + cx,  dy + cy, color);
		OVERLAY_W( dy + cx, -dx + cy, color);
		OVERLAY_W( dx + cx, -dy + cy, color);
		OVERLAY_W(-dy + cx,  dx + cy, color);
		OVERLAY_W(-dx + cx, -dy + cy, color);
		OVERLAY_W(-dy + cx, -dx + cy, color);
	}
}

void draw_point(pointi p, unsigned color) {
	int x = p.x, y = p.y, i;
//void draw_point(int x, int y, unsigned color) {
	OVERLAY_W(x, y, color);
	for (i = 1; i < 5; ++i) {
		OVERLAY_W(x - i, y, color);
		OVERLAY_W(x, y - i, color);
		OVERLAY_W(x + i, y, color);
		OVERLAY_W(x, y + i, color);
	}
}

inline void swap(int *x, int *y) {
	int tmp = *x;
	*x = *y;
	*y = tmp;
}

inline int abs(int x) {
	return x < 0 ? -x : x;
}

void draw_line(pointi p0, pointi p1, unsigned color) {
	int x0 = p0.x, y0 = p0.y;
	int x1 = p1.x, y1 = p1.y;
//void draw_line(int x0, int y0, int x1, int y1, unsigned color) {
	int steep = (abs(y1 - y0) > abs(x1 - x0));
	if (steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}
	int delta_x = x1 - x0, delta_y = abs(y1 - y0);
	int error = delta_x / 2, y_step;
	int x, y = y0;
	if (y0 < y1) y_step = 1;
	else y_step = -1;
	for (x = x0; x <= x1; ++x) {
		if (steep) OVERLAY_W(y, x, color);
		else OVERLAY_W(x, y, color);
		error = error - delta_y;
		if (error < 0) {
			y += y_step;
			error += delta_x;
		}
	}
}

//inline void draw_line_d(int x0, int y0, int delta_x, int delta_y, unsigned color) {
//	draw_line(x0, y0, x0 + delta_x, y0 + delta_y, color);
//}

static float sphere_radius;
static pointi p[8];
static pointf pf[4], df[4];

#define Xf *(float *)&SHARED_R(2)
#define Yf *(float *)&SHARED_R(3)
#define Zf *(float *)&SHARED_R(4)
#define store_pf(idx) (pf[idx].x = Xf, pf[idx].y = Yf, pf[idx].z = Zf)
#define store_df(idx, a, b) (df[idx].x = pf[a].x - pf[b].x, df[idx].y = pf[a].y - pf[b].y, df[idx].z = pf[a].z - pf[b].z)

inline pointf addf(pointf pf, pointf df) {
	pointf ret;
	ret.x = pf.x + df.x;
	ret.y = pf.y + df.y;
	ret.z = pf.z + df.z;
	return ret;
}

void draw_overlay_frame(unsigned color) {
	boolean store = (boolean)(color != TRANSPARENT);
	point cur_p, proj;
	if (store) cur_p = SHARED_R(0);
	
	switch (draw_state) {
		case DRAW_POINT:
			if (store) {
				store_pf(0);
				p[0].x = get_x(cur_p), p[0].y = get_y(cur_p);
			}
			draw_point(p[0], color);
			break;
		case DRAW_SPHERE_RADIUS:
			if (store) {
				store_pf(1);
				store_df(1, 1, 0);
				sphere_radius = sqrt(df[1].x * df[1].x + df[1].y * df[1].y + df[1].z * df[1].z);
			}
			draw_sphere(p[0], sphere_radius, color);
			break;
		case DRAW_CUBE_LINE:
			if (store) {
				store_pf(1);
				store_df(1, 1, 0);
				p[1] = project_point(pf[1]);
			}
			draw_line(p[0], p[1], color);
			break;
		case DRAW_CUBE_AREA:
			if (store) {
				store_pf(2);
				store_df(2, 2, 1);
				p[2] = project_point(addf(pf[0], df[2]));
				p[3] = project_point(pf[2]);
			}
			draw_line(p[0], p[1], color);
			
			draw_line(p[0], p[2], color);
			draw_line(p[1], p[3], color);
			draw_line(p[2], p[3], color);
			break;
		case DRAW_CUBE_VOLUME:
			if (store) {
				store_pf(3);
				store_df(3, 3, 2);
				p[4] = project_point(addf(pf[0], df[3]));
				p[5] = project_point(addf(pf[1], df[3]));
				p[6] = project_point(addf(addf(pf[0], df[2]), df[3]));
				p[7] = project_point(addf(pf[2], df[3]));
			}
			draw_line(p[0], p[1], color);
			
			draw_line(p[0], p[2], color);
			draw_line(p[1], p[3], color);
			draw_line(p[2], p[3], color);
			
			draw_line(p[4], p[5], color);
			draw_line(p[4], p[6], color);
			draw_line(p[5], p[7], color);
			draw_line(p[6], p[7], color);
			draw_line(p[0], p[4], color);
			draw_line(p[1], p[5], color);
			draw_line(p[2], p[6], color);
			draw_line(p[3], p[7], color);
			break;
	}
}

pointi cur_p;

void draw_overlay() {
	drawMsg(overlay_port, last_msg, 20, 20, TRANSPARENT);
	draw_overlay_frame(TRANSPARENT);
	draw_point(cur_p, TRANSPARENT);
	
//	overlay_port = BUFFER_PORT();
	int p = SHARED_R(0);
	cur_p.x = get_x(p), cur_p.y = get_y(p);
	
	drawMsg(overlay_port, msg, 20, 20, WHITE);
	draw_overlay_frame(WHITE);
	draw_point(cur_p, 0xFF0000);
	
//	VSYNC(1);
//	usleep(0);
//	VSYNC(0);
//	int i, j;
//	for (j = 0; j < HEIGHT; ++j)
//		for (i = 0; i < WIDTH; ++i)
//			SDRAM[(2 << 23) | ((j) << 10) | (i)] = SDRAM[(2 << 23) | (1 << 19) | ((j) << 10) | (i)];
}

void key_down(int key_code) {
	printf("key down: %d\n", key_code);
	float dx, dy, dz, radius;
	switch (draw_state) {
		case DRAW_POINT:
			pf[0].x = *(float *)&SHARED_R(2);
			pf[0].y = *(float *)&SHARED_R(3);
			pf[0].z = *(float *)&SHARED_R(4);
			printf("0: (%d,%d,%d)\n", (int)pf[0].x, (int)pf[0].y, (int)pf[0].z);
			if (key_code == 1) draw_state = DRAW_SPHERE_RADIUS;
			else if (key_code == 2) draw_state = DRAW_CUBE_LINE;
			break;
		case DRAW_SPHERE_RADIUS:
			dx = pf[0].x - *(float *)&SHARED_R(2);
			dy = pf[0].y - *(float *)&SHARED_R(3);
			dz = pf[0].z - *(float *)&SHARED_R(4);
			radius = sqrt(dx * dx + dy * dy + dz * dz);
			add_sphere(&pf[0], radius, WHITE);
			draw_overlay_frame(TRANSPARENT);
			draw_state = DRAW_POINT;
			break;
		case DRAW_CUBE_LINE:
			pf[1].x = *(float *)&SHARED_R(2);
			pf[1].y = *(float *)&SHARED_R(3);
			pf[1].z = *(float *)&SHARED_R(4);
			printf("1: (%d,%d,%d)\n", (int)pf[1].x, (int)pf[1].y, (int)pf[1].z);
			draw_state = DRAW_CUBE_AREA;
			break;
		case DRAW_CUBE_AREA:
			pf[2].x = *(float *)&SHARED_R(2);
			pf[2].y = *(float *)&SHARED_R(3);
			pf[2].z = *(float *)&SHARED_R(4);
			printf("2: (%d,%d,%d)\n", (int)pf[2].x, (int)pf[2].y, (int)pf[2].z);
			draw_state = DRAW_CUBE_VOLUME;
			break;
		case DRAW_CUBE_VOLUME:
			pf[3].x = *(float *)&SHARED_R(2);
			pf[3].y = *(float *)&SHARED_R(3);
			pf[3].z = *(float *)&SHARED_R(4);
			printf("3: (%d,%d,%d)\n", (int)pf[3].x, (int)pf[3].y, (int)pf[3].z);
			add_cube(pf, WHITE);
			draw_overlay_frame(TRANSPARENT);
			draw_state = DRAW_POINT;
			break;
	}
}

#endif