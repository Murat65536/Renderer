#include "bitmap.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

bitmap_t create_bitmap(size_t width, size_t height) {
	return (bitmap_t) {width, height, (unsigned int *)malloc(width * height * sizeof(unsigned int))};
}

bitmap_t create_bitmap_file(char *filename) {
	//int fd = open(filename, O_RDONLY);
	//char c;
	//read(fd, &c, 1);
	//printf("%c", c);
	//close(fd);
	return create_bitmap(0, 0);
}

void clear_bitmap(bitmap_t *bm, unsigned char shade) {
	unsigned int color = (shade << 24) | (shade << 16) | (shade << 8) | shade;
	for (unsigned int i = 0; i < bm->width * bm->height; i++) {
		bm->colors[i] = color;
	}
}

void bitmap_set_pixel(bitmap_t *bm, unsigned int x, unsigned int y, unsigned char a, unsigned char b, unsigned char g, unsigned char r) {
	bm->colors[y * bm->width + x] = (a << 24) | (b << 16) | (g << 8) | r;
}
