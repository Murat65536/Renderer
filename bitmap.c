#include "bitmap.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

bitmap newBitmap(unsigned int width, unsigned int height) {
	return (bitmap) {width, height, (unsigned int *)malloc(width * height * sizeof(unsigned int))};
}

bitmap newBitmapFile(char *filename) {
	//int fd = open(filename, O_RDONLY);
	//char c;
	//read(fd, &c, 1);
	//printf("%c", c);
	//close(fd);
	return newBitmap(0, 0);
}

void bitmapClear(bitmap *bm, unsigned char shade) {
	unsigned int color = (shade << 24) | (shade << 16) | (shade << 8) | shade;
	for (unsigned int i = 0; i < bm->width * bm->height; i++) {
		bm->colors[i] = color;
	}
}

void bitmapSetPixel(bitmap *bm, unsigned int x, unsigned int y, unsigned char a, unsigned char b, unsigned char g, unsigned char r) {
	bm->colors[y * bm->width + x] = (a << 24) | (b << 16) | (g << 8) | r;
}
