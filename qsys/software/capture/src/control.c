//
// Created by Kanari on 2016/8/15.
//

//#define CPU_ID 2

#if CPU_ID == 2

#include <unistd.h>

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "memory.h"
#include "common.h"
#include "display.h"
#include "control.h"
#include "render/object.h"

const float focus_x_l = 1117.36809f;
const float focus_x_r = 1098.56402f, focus_y_r = 1095.74358f;
const float center_x_l = 284.432591f;
const float center_x_r = 260.334008f, center_y_r = 307.355809f;
const float stereo_dist = 17.6; // cm

char msg[100], last_msg[100];

Location find_location(point center_l, point center_r) {
	int x_l = get_x(center_l), x_r = get_x(center_r);
	
	Location loc;
	loc.z = stereo_dist / ((x_l - center_x_l) / focus_x_l - (x_r - center_x_r) / focus_x_r);
	loc.x = loc.z * (x_r - center_x_r) / focus_x_r;
	loc.y = loc.z * (get_y(center_r) - center_y_r) / focus_y_r;
	
	memcpy(last_msg, msg, sizeof(msg));
	sprintf(msg, "Left (%3d,%3d)\nRight (%3d,%3d)\nPosition (%3d,%3d,%3d)",
	        get_x(center_l), get_y(center_l),
	        get_x(center_r), get_y(center_r),
	        (int)loc.x, (int)loc.y, (int)loc.z);
	
	return loc;
}

enum DRAW_STATE {
	DRAW_SPHERE_CENTER,
	DRAW_SPHERE_RADIUS
} draw_state;

float sphere_x, sphere_y, sphere_z;
point sphere_center;
float sphere_radius;

//#define OVERLAY_W(x, y, val) (SDRAM[(2 << 23) | (0 << 19) | ((y) << 10) | (x)] = (val))
#define OVERLAY_W(x, y, val) (SDRAM[(1 << 23) | (overlay_port << 19) | ((y) << 10) | (x)] = (val))

unsigned overlay_port = 0;

void draw_sphere(point center, float radius, unsigned color) {
	int cx = get_x(center), cy = get_y(center);
	int dx = 0, dy = (int)radius;
	double d = 1.25 - radius;
	while (dx <= dy) {
		if (d < 0) d += 2 * dx + 3;
		else d += 2 * (dx - dy) + 5, --dy;
		++dx;
		
		OVERLAY_W(dx + cx, dy + cy, color);
		OVERLAY_W(dy + cx, dx + cy, color);
		OVERLAY_W(-dx + cx, dy + cy, color);
		OVERLAY_W(dy + cx, -dx + cy, color);
		OVERLAY_W(dx + cx, -dy + cy, color);
		OVERLAY_W(-dy + cx, dx + cy, color);
		OVERLAY_W(-dx + cx, -dy + cy, color);
		OVERLAY_W(-dy + cx, -dx + cy, color);
	}
}

void draw_point(point center, unsigned color) {
	int x = get_x(center), y = get_y(center);
	OVERLAY_W(x, y, color);
	if (x > 0) OVERLAY_W(x - 1, y, color);
	if (y > 0) OVERLAY_W(x, y - 1, color);
	if (x < WIDTH - 1) OVERLAY_W(x + 1, y, color);
	if (y < HEIGHT - 1) OVERLAY_W(x, y + 1, color);
}

void draw_overlay() {
	point p = SHARED_R(0);
	int dx, dy;
	
	drawMsg(overlay_port, last_msg, 20, 20, TRANSPARENT);
	switch (draw_state) {
		case DRAW_SPHERE_CENTER:
			draw_point(sphere_center, TRANSPARENT);
			break;
		case DRAW_SPHERE_RADIUS:
			draw_sphere(sphere_center, sphere_radius, TRANSPARENT);
			break;
	}
	
	overlay_port = IORD(RENDER_PORT_PIO_BASE, 0);
	
	drawMsg(overlay_port, msg, 20, 20, WHITE);
	switch (draw_state) {
		case DRAW_SPHERE_CENTER:
			sphere_center = p;
			draw_point(sphere_center, WHITE);
			break;
		case DRAW_SPHERE_RADIUS:
			dx = get_x(p) - get_x(sphere_center);
			dy = get_y(p) - get_y(sphere_center);
			sphere_radius = sqrt(dx * dx + dy * dy);
			draw_sphere(sphere_center, sphere_radius, WHITE);
			break;
	}
}

void key_down(int key_code) {
	float dx, dy, dz, radius;
	switch (draw_state) {
		case DRAW_SPHERE_CENTER:
			sphere_x = *(float *)&SHARED_R(2);
			sphere_y = *(float *)&SHARED_R(3);
			sphere_z = *(float *)&SHARED_R(4);
			draw_state = DRAW_SPHERE_RADIUS;
			break;
		case DRAW_SPHERE_RADIUS:
			dx = sphere_x - *(float *)&SHARED_R(2);
			dy = sphere_y - *(float *)&SHARED_R(3);
			dz = sphere_z - *(float *)&SHARED_R(4);
			radius = sqrt(dx * dx + dy * dy + dz * dz);
			add_sphere(sphere_x, sphere_y, sphere_z, radius, WHITE);
			draw_point(sphere_center, TRANSPARENT);
			draw_sphere(sphere_center, sphere_radius, TRANSPARENT);
			draw_state = DRAW_SPHERE_CENTER;
			break;
	}
}

#endif