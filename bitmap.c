#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"
#include "lodepng.h"

bitmap_t *create_bitmap(char *filename) {
	bitmap_t *bitmap = malloc(sizeof(bitmap_t));
	lodepng_decode24_file(&bitmap->colors, &bitmap->width, &bitmap->height, filename);
	return bitmap;
}

void free_bitmap(bitmap_t *bitmap) {
	free(bitmap->colors);
	free(bitmap);
}
