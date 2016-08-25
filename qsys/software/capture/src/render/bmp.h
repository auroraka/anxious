#ifndef BMP_H
#define BMP_H

#include"color.h"
#include <string.h>

extern const float EPS;

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned int dword;

struct BITMAPFILEHEADER {
	dword bfSize;
	word bfReserved1;
	word bfReserved2;
	dword bfOffBits;
};

struct BITMAPINFOHEADER {
	dword biSize;
	long biWidth;
	long biHeight;
	word biPlanes;
	word biBitCount;
	dword biCompression;
	dword biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	dword biClrUsed;
	dword biClrImportant;
};

struct RGBQUAD {
	byte rgbBlue;
	byte rgbGreen;
	byte rgbRed;
	byte rgbReserved;
};

struct IMAGEDATA {
	byte red;
	byte green;
	byte blue;
};

struct Bmp {
	struct BITMAPFILEHEADER strHead;
	struct BITMAPINFOHEADER strInfo;
	int ima_created;
	struct IMAGEDATA ima[480][640];
	
	
} BMP;


void SetColor(struct Bmp *b, int i, int j, Color col) {
	b->ima[i][j].red = (int)(col[0] * 255);
	b->ima[i][j].green = (int)(col[1] * 255);
	b->ima[i][j].blue = (int)(col[2] * 255);
}

void Initialize(struct Bmp *b) {
	int H = PIC_H;
	int W = PIC_W;
	b->strHead.bfReserved1 = 0;
	b->strHead.bfReserved2 = 0;
	b->strHead.bfOffBits = 54;
	
	b->strInfo.biSize = 40;
	b->strInfo.biPlanes = 1;
	b->strInfo.biHeight = H;
	b->strInfo.biWidth = W;
	b->strInfo.biBitCount = 24;
	b->strInfo.biCompression = 0;
	b->strInfo.biSizeImage = H * W * 3;
	b->strInfo.biXPelsPerMeter = 0;
	b->strInfo.biYPelsPerMeter = 0;
	b->strInfo.biClrUsed = 0;
	b->strInfo.biClrImportant = 0;
	
	b->strHead.bfSize = b->strInfo.biSizeImage + b->strInfo.biBitCount;
	
}

void Output(struct Bmp *b, char file[]) {
	FILE *fpw = fopen(file, "wb");
	
	word bfType = 0x4d42;
	fwrite(&bfType, 1, sizeof(word), fpw);
	fwrite(&b->strHead, 1, sizeof(struct BITMAPFILEHEADER), fpw);
	fwrite(&b->strInfo, 1, sizeof(struct BITMAPINFOHEADER), fpw);
	
	for (int i = 0; i < b->strInfo.biHeight; i++)
		for (int j = 0; j < b->strInfo.biWidth; j++) {
			fwrite(&b->ima[i][j].blue, 1, sizeof(byte), fpw);
			fwrite(&b->ima[i][j].green, 1, sizeof(byte), fpw);
			fwrite(&b->ima[i][j].red, 1, sizeof(byte), fpw);
		}
	
	fclose(fpw);
}

#endif
