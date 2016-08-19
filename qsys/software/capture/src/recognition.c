//
// Created by Kanari on 2016/8/15.
//

#include <system.h>
#include <unistd.h>
#include <io.h>

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "recognition.h"

#define MEMORY_SIZE 8192

#define RAW_HEIGHT (480)
#define RAW_WIDTH (640)
#define WIDTH (RAW_WIDTH >> 1)
#define HEIGHT (RAW_HEIGHT >> 1)
const unsigned MASK_WIDTH = WIDTH >> 5;

#define RECORD_BORDER 0

unsigned cam_bank, render_bank;

#define SDRAM_R(x, y) sdram[(cam_bank << 19) | ((y) << 10) | (x)]
#define SDRAM_W(x, y, val) (sdram[(1 << 23) | (render_bank << 19) | ((y) << 10) | (x)] = (val))
#define SDRAM_CLEAR(x, y) (sdram[(1 << 23) | (render_bank << 19) | ((y) << 10) | (x)] = (1 << 24))

#define sdram ((volatile unsigned *)  SDRAM_CONTROLLER_0_BASE)

#define get_frame(x, y) ((frame[(y) * MASK_WIDTH + ((x) >> 5)] >> ((x) & 31)) & 1)
#define set_frame_0(x, y) (frame[(y) * MASK_WIDTH + ((x) >> 5)] &= ~(1 << ((x) & 31)))
#define set_frame_1(x, y) (frame[(y) * MASK_WIDTH + ((x) >> 5)] |= 1 << ((x) & 31))
#define set_frame(x, y, val) ((frame[(y) * MASK_WIDTH + ((x) >> 5)] &= ~(1 << ((x) & 31))), (frame[(y) * MASK_WIDTH + ((x) >> 5)] |= (val) << ((x) & 31)))

unsigned max(unsigned x, unsigned y) {
	return x ^ ((x ^ y) & -(x < y));
}

unsigned min(unsigned x, unsigned y) {
	return y ^ ((x ^ y) & -(x < y));
}

const unsigned H_l = 29, S_l = 86, V_l = 6;
const unsigned H_r = 64, S_r = 255, V_r = 255;

unsigned check_range(unsigned pixel) {
	register unsigned R, G, B, H, S, V, delta;
	
	R = ((pixel >> 20) & 0x3FF) >> 2;
	G = ((pixel >> 10) & 0x3FF) >> 2;
	B = (pixel & 0x3FF) >> 2;
	V = max(max(R, G), B);
	if (V < V_l || V_r < V) return 0;
	
	delta = V - min(min(R, G), B);
	if (!delta) return 0;
	S = ((delta << 8) - 1) / V;
	if (S < S_l || S_r < S) return 0;

	// coef = 60 * 256 / 360 = 42.6667
	if (R == V) H = (unsigned)(((int)G - (int)B) * 42 / (int)delta + 256);
	else if (G == V) H = (unsigned)(((int)B - (int)R) * 42 / (int)delta + 85 + 256);
	else H = (unsigned)(((int)R - (int)G) * 42 / (int)delta + 171 + 256);
	H &= 255;
	H = H * 180 >> 8;
	if (H < H_l || H_r < H) return 0;
	return 1;
}

void cvtColor_inRange(int bank, unsigned *frame, unsigned *row) {
	int i, j;
	unsigned pixel;
	
	cam_bank = bank;
	
	for (j = 0; j < RAW_HEIGHT; ++j) {
		if ((j & 1) == 0) {
			for (i = 0; i < WIDTH; ++i)
				row[i] = 0;
		}
		for (i = 0; i < RAW_WIDTH; ++i) {
			pixel = SDRAM_R(i, j);
			row[i >> 1] += (((pixel >> 16) & 0xFF) << 20)
			               + (((pixel >> 8) & 0xFF) << 10)
			               + (pixel & 0xFF);
		}
		if ((j & 1) == 1) {
			for (i = 0; i < WIDTH; ++i)
				set_frame(i, j >> 1, check_range(row[i]));
		}
	}
}

void erode(unsigned *frame, unsigned *row) {
	unsigned *first = row, *last = row + MASK_WIDTH, *swp_tmp;
	unsigned pixel;
	int delta, i, j;
	
	for (j = 0; j < HEIGHT; ++j) {
		delta = j * MASK_WIDTH;
		pixel = frame[delta];
		last[0] = pixel & ((pixel >> 1) | (frame[delta + 1] << 31)) & ((pixel << 1) | 1);
		for (i = 1; i < MASK_WIDTH - 1; ++i) {
			pixel = frame[delta + i];
			last[i] = pixel & ((pixel >> 1) | (frame[delta + i + 1] << 31)) & ((pixel << 1) | (frame[delta + i - 1] >> 31));
		}
		pixel = frame[delta + MASK_WIDTH - 1];
		last[MASK_WIDTH - 1] = pixel & ((pixel >> 1) | (1 << 31)) & ((pixel << 1) | (frame[delta + MASK_WIDTH - 2] >> 31));
		memcpy(frame + delta, last, MASK_WIDTH * sizeof(unsigned));
	}
	memset(first, 0xFF, MASK_WIDTH * sizeof(unsigned));
	for (j = 0; j < HEIGHT; ++j) {
		delta = j * MASK_WIDTH;
		memcpy(last, frame + delta, MASK_WIDTH * sizeof(unsigned));
		for (i = 0; i < MASK_WIDTH; ++i)      // valid overflow in last row
			frame[delta + i] &= frame[delta + MASK_WIDTH + i] & first[i];
		swp_tmp = first;
		first = last;
		last = swp_tmp;
	}
}

void dilate(unsigned *frame, unsigned *row) {
	unsigned *first = row, *last = row + MASK_WIDTH, *swp_tmp;
	unsigned pixel;
	int delta, i, j;
	
	for (j = 0; j < HEIGHT; ++j) {
		delta = j * MASK_WIDTH;
		pixel = frame[delta];
		last[0] = pixel | ((pixel >> 1) | (frame[delta + 1] << 31)) | (pixel << 1);
		for (i = 1; i < MASK_WIDTH - 1; ++i) {
			pixel = frame[delta + i];
			last[i] = pixel | ((pixel >> 1) | (frame[delta + i + 1] << 31)) | ((pixel << 1) | (frame[delta + i - 1] >> 31));
		}
		pixel = frame[delta + MASK_WIDTH - 1];
		last[MASK_WIDTH - 1] = pixel | (pixel >> 1) | ((pixel << 1) | (frame[delta + MASK_WIDTH - 2] >> 31));
		memcpy(frame + delta, last, MASK_WIDTH * sizeof(unsigned));
	}
	memset(first, 0x00, MASK_WIDTH * sizeof(unsigned));
	for (j = 0; j < HEIGHT; ++j) {
		delta = j * MASK_WIDTH;
		memcpy(last, frame + delta, MASK_WIDTH * sizeof(unsigned));
		for (i = 0; i < MASK_WIDTH; ++i)      // valid overflow in last row
			frame[delta + i] |= frame[delta + MASK_WIDTH + i] | first[i];
		swp_tmp = first;
		first = last;
		last = swp_tmp;
	}
}

// dir : 0 -> from (row+1); 1 -> from (row-1)
#define line(dir, row, l, r) (unsigned)(((dir) << 30) | ((row) << 20) | ((l) << 10) | (r))
#define get_dir(p) (((p) >> 30) & 1)
#define get_row(p) (((p) >> 20) & 0x1FF)
#define get_l(p) (((p) >> 10) & 0x3FF)
#define get_r(p) ((p) & 0x3FF)

#define QUEUE_SIZE (WIDTH * 2)
#define MAX_BORDER (MEMORY_SIZE - HEIGHT * MASK_WIDTH - QUEUE_SIZE)
unsigned *queue;
int head, tail;

#if RECORD_BORDER
unsigned *border = row + QUEUE_SIZE;
int border_len;

void addBorder(unsigned p) {
	if (border_len < MAX_BORDER)
		border[border_len++] = p;
}
#endif

int inc(int *x) {
	*x = *x + 1 == QUEUE_SIZE ? 0 : *x + 1;
	assert(x == &head || tail != head);
	return *x;
}

int scanRow(unsigned *frame, unsigned *queue, int dir, int row, int l, int r) {
	int i, start = 0, area = 0;
#if RECORD_BORDER
	int row_from = dir ? row - 1 : row + 1;
#endif
	boolean last = false;
	for (i = l; i <= r; ++i) {
		if (get_frame(i, row)) {
			set_frame_0(i, row);
			++area;
			if (!last) {
				start = i;
				last = true;
			}
		} else {
			if (last) {
				queue[inc(&tail)] = line(dir, row, start, i - 1);
				last = false;
			}
#if RECORD_BORDER
			addBorder(point(i, row_from));
#endif
		}
	}
	if (last) queue[inc(&tail)] = line(dir, row, start, r);
	return area;
}

int findContour(unsigned *frame, unsigned *queue, int sx, int sy, unsigned *pt1, unsigned *pt2) {
	int dir, row, l_from, r_from, l, r, area = 0;
	int min_x = WIDTH, max_x = 0, min_y = HEIGHT, max_y = 0;
	
	head = tail = 0;
#if RECORD_BORDER
	border_len = 0;
#endif
	
	queue[inc(&tail)] = line(1, sy, sx, sx);
	set_frame_0(sx, sy);
	area = 1;
	if (sy > 0 && get_frame(sx, sy - 1)) {
		queue[inc(&tail)] = line(0, sy - 1, sx, sx);
		set_frame_0(sx, sy - 1);
		area = 2;
	}
	while (head != tail) {
		inc(&head);
		dir = get_dir(queue[head]);
		row = get_row(queue[head]);
		l_from = get_l(queue[head]);
		r_from = get_r(queue[head]);
		assert(l_from <= r_from);
		min_x = min(min_x, l_from);
		max_x = max(max_x, r_from);
		min_y = min(min_y, row);
		max_y = max(max_y, row);
		
		for (l = l_from - 1; l >= 0; --l) {
			if (!get_frame(l, row)) break;
			set_frame_0(l, row);
		}
		++l;
		area += l_from - l;
		for (r = r_from + 1; r < WIDTH; ++r) {
			if (!get_frame(r, row)) break;
			set_frame_0(r, row);
		}
		--r;
		area += r - r_from;
#if RECORD_BORDER
		addBorder(point(l, row));
		addBorder(point(r, row));
#endif
		
		if (row > 0) {
			if (dir != 1) area += scanRow(frame, queue, 0, row - 1, l, r);
			else  {
				area += scanRow(frame, queue, 0, row - 1, l, l_from - 1);
				area += scanRow(frame, queue, 0, row - 1, r_from + 1, r);
			}
		}
		if (row < HEIGHT - 1) {
			if (dir != 0) area += scanRow(frame, queue, 1, row + 1, l, r);
			else {
				area += scanRow(frame, queue, 1, row + 1, l, l_from - 1);
				area += scanRow(frame, queue, 1, row + 1, r_from + 1, r);
			}
		}
	}

#if RECORD_BORDER
	for (int i = 0; i < border_len; ++i) {
		int x = get_x(border[i]);
		int y = get_y(border[i]);
		assert(min_x <= x && x <= max_x && min_y <= y && y <= max_y);
		frame_out.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
	}

	cv::rectangle(frame_out, cv::Point(min_x, min_y), cv::Point(max_x, max_y), cv::Scalar(0, 255, 0));
	cv::circle(frame_out, cv::Point((min_x + max_x) >> 1, (min_y + max_y) >> 1), 2, cv::Scalar(255, 0, 0));
#endif
	
	*pt1 = point(min_x, min_y);
	*pt2 = point(max_x, max_y);
	
	return area;
}

RecogResult floodfill(unsigned *frame, unsigned *queue) {
	int i, j, best_area = 0, cur_area;
	unsigned best_pt1 = 0, best_pt2 = 0, cur_pt1, cur_pt2;
	int min_x, max_x, min_y, max_y, center_x, center_y;
	RecogResult result;
	
	for (j = 0; j < HEIGHT; ++j)
		for (i = 0; i < WIDTH; ++i)
			if (get_frame(i, j)) {
				cur_area = findContour(frame, queue, i, j, &cur_pt1, &cur_pt2);
				if (cur_area > best_area) {
					best_area = cur_area;
					best_pt1 = cur_pt1;
					best_pt2 = cur_pt2;
				}
			}
	
	min_x = get_x(best_pt1) * 2;
	min_y = get_y(best_pt1) * 2;
	max_x = get_x(best_pt2) * 2 + 1;
	max_y = get_y(best_pt2) * 2 + 1;
	center_x = (min_x + max_x) / 2;
	center_y = (min_y + max_y) / 2;
	result.rect_lu = point(min_x, min_y);
	result.rect_rd = point(max_x, max_y);
	result.center = point(center_x, center_y);
	return result;
}

#undef line
#undef get_dir
#undef get_row
#undef get_l
#undef get_r

RecogResult recognize(int bank, unsigned *frame, unsigned *tmp) {
	cvtColor_inRange(bank, frame, tmp);
	erode(frame, tmp);
	dilate(frame, tmp);
	return floodfill(frame, tmp);
}

void clearResult(int bank, RecogResult *result) {
	int min_x, max_x, min_y, max_y, center_x, center_y, i, j;
	unpack_result(result);
	render_bank = bank;
	
	for (i = min_x; i <= max_x; ++i) {
		SDRAM_CLEAR(i, min_y);
		SDRAM_CLEAR(i, max_y);
	}
	for (i = min_y; i <= max_y; ++i) {
		SDRAM_CLEAR(min_x, i);
		SDRAM_CLEAR(max_x, i);
	}
	for (i = center_x - 1; i <= center_x + 2; ++i)
		for (j = center_y - 1; j <= center_y + 2; ++j)
			SDRAM_CLEAR(i, j);
}

void drawResult(int bank, RecogResult *result) {
	int min_x, max_x, min_y, max_y, center_x, center_y, i, j;
	unpack_result(result);
	render_bank = bank;
	
	for (i = min_x; i <= max_x; ++i) {
		SDRAM_W(i, min_y, 255 << 8);
		SDRAM_W(i, max_y, 255 << 8);
	}
	for (i = min_y; i <= max_y; ++i) {
		SDRAM_W(min_x, i, 255 << 8);
		SDRAM_W(max_x, i, 255 << 8);
	}
	for (i = center_x - 1; i <= center_x + 2; ++i)
		for (j = center_y - 1; j <= center_y + 2; ++j)
			SDRAM_W(i, j, 255);
}