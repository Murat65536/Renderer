#ifndef BITMAP_H_
#define BITMAP_H_


typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int *colors;
} bitmap;

bitmap newBitmap(unsigned int width, unsigned int height);

bitmap newBitmapFile(char *filename);

void bitmapClear(bitmap *bm, unsigned char shade);

void bitmapSetPixel(bitmap *bm, unsigned int x, unsigned int y, unsigned char a, unsigned char b, unsigned char g, unsigned char r);

#endif
