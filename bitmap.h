#ifndef BITMAP_H_
#define BITMAP_H_

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned char *colors;
} bitmap_t;

bitmap_t *create_bitmap(unsigned int width, unsigned int height);

bitmap_t *create_bitmap_file(char *filename);

#endif
