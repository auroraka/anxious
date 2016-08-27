//
// Created by Kanari on 2016/8/15.
//

#ifndef ANXIOUS_CONTROL_H
#define ANXIOUS_CONTROL_H

#include "common.h"

#define KEY_R() IORD(KEY_PIO_BASE, 0)
#define IR_R() ((IORD(IR_RECEIVER_0_BASE, 0) >> 16) & 0xFF)

typedef struct {
	float x, y, z;
} Location;

Location find_location(point center_l, point center_r);

void draw_overlay();

void key_down(int key_code);

#endif //CONTROL_H
