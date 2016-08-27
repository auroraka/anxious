//
// Created by Kanari on 2016/8/24.
//

#ifndef ANXIOUS_PALETTE_H
#define ANXIOUS_PALETTE_H

#include "common.h"

#define WINDOW_BG_COLOR 0xECECEC
#define WINDOW_BORDER_COLOR 0xCCCCCC

#define PALETTE_SIZE 12
static const unsigned palette_colors[PALETTE_SIZE] = {
		0x3F51B5, // Indigo     W
		0x00BCD4, // Cyan       B
		0xEC407A, // Pink       W
		0xAB47BC, // Violet     W
		0x009688, // Teal       W
		0x64FFDA, // Pale Teal  W
		0x4CAF50, // Green      B
		0x2196F3, // Blue       W
		0xFFD740, // Amber      B
		0x607D8B, // Blue Grey  W
		0x424242, // Grey       W
		0xECEFF1, // White      B
};
static const char *palette_names[PALETTE_SIZE] = {
		"Indigo",
        "Cyan",
        "Pink",
        "Violet",
        "Teal",
        "Pale Teal",
        "Green",
        "Blue",
        "Amber",
        "Blue Grey",
        "Grey",
        "White",
};
static const boolean palette_dark[PALETTE_SIZE] = {
		true,
        false,
        true,
        true,
        true,
        false,
        false,
        true,
        false,
        true,
        true,
        false,
};

#endif //PALETTE_H
