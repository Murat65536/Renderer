#ifndef BITMAP_H_
#define BITMAP_H_
#include <stddef.h>

typedef struct {
	size_t width;
	size_t height;
	unsigned int *colors;
} bitmap_t;

bitmap_t create_bitmap(size_t width, size_t height);

bitmap_t create_bitmap_file(char *filename);

void clear_bitmap(bitmap_t *bm, unsigned char shade);

void bitmap_set_pixel(bitmap_t *bm, unsigned int x, unsigned int y, unsigned char a, unsigned char b, unsigned char g, unsigned char r);

#endif
