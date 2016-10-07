//
// Created by Kanari on 2016/8/22.
//

#ifndef ANXIOUS_COMMON_H
#define ANXIOUS_COMMON_H

#define NDEBUG 1

#define RENDER_CORES 1

typedef unsigned char boolean;
#define true 1
#define false 0

typedef int point;

#define point(x, y) (unsigned)(((y) << 16) | (x))
#define get_x(p) (unsigned)((p) & 0xFFFF)
#define get_y(p) (unsigned)(((p) >> 16) & 0xFFFF)

#define TRANSPARENT (1 << 24)
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define RED 0xFF0000

#define HEIGHT (480)
#define WIDTH (640)
#define PIC_H HEIGHT
#define PIC_W WIDTH

#endif // COMMON_H
