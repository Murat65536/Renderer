#ifndef BITMAP_H_
#define BITMAP_H_

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned char *colors;
} bitmap_t;

bitmap_t *create_bitmap(char *filename);

void free_bitmap(bitmap_t *bitmap);

#endif
