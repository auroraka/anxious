//
// Created by Kanari on 2016/8/15.
//

#ifndef ANXIOUS_RECOGNITION_H
#define ANXIOUS_RECOGNITION_H

#include "common.h"

#define unpack_result(result) \
    ((min_x) = get_x((result)->rect_lu), \
     (min_y) = get_y((result)->rect_lu), \
     (max_x) = get_x((result)->rect_rd), \
     (max_y) = get_y((result)->rect_rd), \
     (center_x) = get_x((result)->center), \
     (center_y) = get_y((result)->center))

typedef struct {
	point rect_lu, rect_rd;
	point center;
} RecogResult;

void erode(unsigned *frame, unsigned *temp);

void dilate(unsigned *frame, unsigned *temp);

RecogResult recognize_raw(unsigned port);

RecogResult recognize();

void clear_result(unsigned port, RecogResult *result);

void draw_result(unsigned port, RecogResult *result);

#endif //RECOGNITION_H
