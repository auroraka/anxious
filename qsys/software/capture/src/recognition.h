//
// Created by Kanari on 2016/8/15.
//

#ifndef RECOGNITION_H
#define RECOGNITION_H

#define unpack_result(result) \
	((min_x) = get_x((result)->rect_lu), \
	 (min_y) = get_y((result)->rect_lu), \
	 (max_x) = get_x((result)->rect_rd), \
	 (max_y) = get_y((result)->rect_rd), \
	 (center_x) = get_x((result)->center), \
	 (center_y) = get_y((result)->center)) \

#include "common.h"

typedef struct {
	point rect_lu, rect_rd;
	point center;
} RecogResult;

void erode(unsigned *frame, unsigned *temp);

void dilate(unsigned *frame, unsigned *temp);

RecogResult recognize(int bank, unsigned *frame, unsigned *tmp);

void clearResult(int bank, RecogResult *result);

void drawResult(int bank, RecogResult *result);

#endif //RECOGNITION_H
