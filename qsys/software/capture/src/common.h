//
// Created by Kanari on 2016/8/22.
//

#ifndef COMMON_H
#define COMMON_H

#define NDEBUG 1

#define RENDER_CORES 1

typedef unsigned char boolean;
#define true 1
#define false 0

typedef int point;

#define point(x, y) (unsigned)(((y) << 10) | (x))
#define get_x(p) (((p) >> 10) & 0x3FF)
#define get_y(p) ((p) & 0x3FF)

#endif // COMMON_H
