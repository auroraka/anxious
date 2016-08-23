//
// Created by Kanari on 2016/8/15.
//

#include <unistd.h>

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "memory.h"
#include "common.h"
#include "display.h"
#include "control.h"

const float focus_x_l = 1117.36809f;
const float focus_x_r = 1098.56402f, focus_y_r = 1095.74358f;
const float center_x_l = 284.432591f;
const float center_x_r = 260.334008f, center_y_r = 307.355809f;
const float stereo_dist = 17.6; // cm

char msg[100];

Location find_location(point center_l, point center_r, boolean show_msg) {
	int x_l = get_x(center_l), x_r = get_x(center_r);
	
	Location loc;
	loc.z = stereo_dist / ((x_l - center_x_l) / focus_x_l - (x_r - center_x_r) / focus_x_r);
	loc.x = loc.z * (x_r - center_x_r) / focus_x_r;
	loc.y = loc.z * (get_y(center_r) - center_y_r) / focus_y_r;
	
	if (show_msg) {
		clearMsg(0, msg, 20, 20);
		sprintf(msg, "Left (%3d,%3d)\nRight (%3d,%3d)\nPosition (%3d,%3d,%3d)",
		        get_x(center_l), get_y(center_l),
		        get_x(center_r), get_y(center_r),
		        (int)loc.x, (int)loc.y, (int)loc.z);
		drawMsg(0, msg, 20, 20);
	}
	
	return loc;
}

enum DRAW_STATE {
	DRAW_SPHERE_CENTER,
	DRAW_SPHERE_RADIUS
} draw_state;

point sphere_center;
float sphere_radius;

void clear_overlay() {
	
}

void draw_overlay() {
	point p = SHARED_R(0);
	clear_overlay();
	switch (draw_state) {
		case DRAW_SPHERE_CENTER:
			sphere_center = p;
			break;
		case DRAW_SPHERE_RADIUS:
			int dx = get_x(p) - get_x(sphere_center);
			int dy = get_y(p) - get_y(sphere_center);
			sphere_radius = sqrt(dx * dx + dy * dy);
			break;
	}
	
}

void key_down(int key_code) {
	switch (draw_state) {
		case DRAW_SPHERE_CENTER:
			draw_state = DRAW_SPHERE_RADIUS;
			break;
		case DRAW_SPHERE_RADIUS:
			
			draw_state = DRAW_SPHERE_CENTER;
			break;
	}
}
