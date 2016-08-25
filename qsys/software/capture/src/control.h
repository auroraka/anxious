//
// Created by Kanari on 2016/8/15.
//

#ifndef CONTROL_H
#define CONTROL_H

#include "common.h"

typedef struct {
	float x, y, z;
} Location;

Location find_location(point center_l, point center_r, boolean show_msg);

void draw_overlay();

void key_down(int key_code);

#endif //CONTROL_H
