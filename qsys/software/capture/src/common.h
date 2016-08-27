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

#define point(x, y) (unsigned)(((y) << 16) | (x))
#define get_x(p) (unsigned)((p) & 0xFFFF)
#define get_y(p) (unsigned)(((p) >> 16) & 0xFFFF)

#endif // COMMON_H
