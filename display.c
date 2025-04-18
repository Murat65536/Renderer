#include "display.h"
#include "matrix.h"
#include "bitmap.h"
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <stdbool.h>
#include <signal.h>
#include <wchar.h>
#include <locale.h>

#define RAND_MAX 255

int columns, rows;
wchar_t *buffer;
wchar_t *clearBuffer;

int newFd;

struct termios origTermios;

matrix projection;

bitmap texture;

bool update = true;

void resetTerminalMode() {
	write(newFd, "\x1b[0m\x1b[2J\x1b[?25h\x1b[8;100;200t", 27);
	tcsetattr(0, TCSANOW, &origTermios);
	free(buffer);
	free(clearBuffer);
	free(texture.colors);
}

void queueResUpdate() {
	update = true;
}

void resUpdate() {	
	if (update) {
		update = false;
		struct winsize winsz;
		ioctl(0, TIOCGWINSZ, &winsz);
		columns = winsz.ws_col;
		rows = winsz.ws_row;
		projection = initPerspective(70.f * 180.f / 3.14159f, columns / rows * 0.5f, 0.1f, 1000.f);
		free(buffer);
		free(clearBuffer);
		buffer = (wchar_t *)malloc(((columns * rows) * 37 + 5) * sizeof(wchar_t));
		clearBuffer = (wchar_t *)malloc(((columns * rows) * 37 + 5) * sizeof(wchar_t));

		for (int i = 0; i < columns * rows; i++) {
			wcsncpy(buffer + i * 37, L"\x1b[38;2;000;000;000;48;2;000;000;000m▀", 38);
		}
	
		wcsncpy(buffer + columns * rows * 37, L"\x1b[;H", 5);
		wcsncpy(clearBuffer, buffer, rows * columns * 37 + 5);
	}
}

void initDisplay() {
	setlocale(LC_ALL, "");
	resUpdate();
	signal(SIGWINCH, queueResUpdate);
	newFd = dup(STDOUT_FILENO);
	
	write(newFd, "\x1b[?25l", 7);
	
	texture = newBitmap(32, 32);
	for (unsigned int j = 0; j < texture.height; j++) {
		for (unsigned int i = 0; i < texture.width; i++) {
			bitmapSetPixel(&texture, i, j, 255, random(), random(), random());
		}
	}
 

	struct termios newTermios;
	tcgetattr(0, &origTermios);
	memcpy(&newTermios, &origTermios, sizeof(newTermios));
	atexit(resetTerminalMode);
	cfmakeraw(&newTermios);
	tcsetattr(0, TCSANOW, &newTermios);
}

matrix getProjectionMatrix() {
	return projection;
}

bool kbhit() {
	struct timeval tv = {0L, 0L};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv) > 0;
}

int getch() {
	int r;
	unsigned char c;
	if ((r = read(0, &c, sizeof(c))) < 0) {
		return r;
	}
	else {
		return c;
	}
}

void render() {
	wprintf(L"%ls", buffer, rows * columns * 37 + 4);
}

void clear() {
	wcsncpy(buffer, clearBuffer, rows * columns * 37 + 5);
}

unsigned int getRGB(unsigned char r, unsigned char g, unsigned char b) {
	return (b << 16) | (g << 8) | r;
}

void plotPoint(unsigned int x, unsigned int y, unsigned int color) {
	for (char i = 0; i < 3; i++) {
		for (char digit = 0; digit < 3; digit++) {
			char offset;
			if (y % 2 == 0) {
				offset = 9;
			}
			else {
				offset = 26;
			}
			buffer[(y / 2 * columns + x) * 37 + 4 * i + offset - digit] = 48 + ((color >> 8 * i) & 0xFF) / (int)pow(10, digit) % 10;
		}
	}
}

void drawScanLine(edge *left, edge *right, int j) {
	int xMin = (int)ceil(left->x);
	int xMax = (int)ceil(right->x);

	float xPrestep = xMin - left->x;

	float xDist = right->x - left->x;
	float texCoordXXStep = (right->texCoordX - left->texCoordX) / xDist;
	float texCoordYXStep = (right->texCoordY - left->texCoordY) / xDist;
	float oneOverZXStep = (right->oneOverZ - left->oneOverZ) / xDist;

	float texCoordX = left->texCoordX + texCoordXXStep * xPrestep;
	float texCoordY = left->texCoordY + texCoordYXStep * xPrestep;
	float oneOverZ = left->oneOverZ + oneOverZXStep * xPrestep;

	for (int i = xMin; i < xMax; i++) {
		float z = 1.f / oneOverZ;
		int srcX = (int)((texCoordX * z) * (texture.width - 1) + 0.5f);
		int srcY = (int)((texCoordY * z) * (texture.height - 1) + 0.5f);
		plotPoint(i, j, texture.colors[srcY * texture.width + srcX]);
	
		oneOverZ += oneOverZXStep;
		texCoordX += texCoordXXStep;
		texCoordY += texCoordYXStep;
	}
}

void scanEdges(edge *a, edge *b, bool side) {
	edge *left;
	edge *right;

	if (side) {
		left = b;
		right = a;
	}
	else {
		left = a;
		right = b;
	}

	int yStart = b->yStart;
	int yEnd = b->yEnd;

	for (int j = yStart; j < yEnd; j++) {
		drawScanLine(left, right, j);
		edgeStep(left);
		edgeStep(right);
	}
}

void scanTriangle(vertex minYVert, vertex midYVert, vertex maxYVert, bool side) {
	gradients g = newGradients(minYVert, midYVert, maxYVert);
	edge topToBottom = newEdge(g, minYVert, maxYVert, 0);
	edge topToMiddle = newEdge(g, minYVert, midYVert, 0);
	edge middleToBottom = newEdge(g, midYVert, maxYVert, 1);
	
	scanEdges(&topToBottom, &topToMiddle, side);
	scanEdges(&topToBottom, &middleToBottom, side);
}


void fillTriangle(vertex v1, vertex v2, vertex v3) {
	matrix screenSpace = initScreenSpaceTransform(columns * 0.5f, rows);
	vertex minYVert = vertexPerspectiveDivide(vertexTransform(v1, screenSpace));
	vertex midYVert = vertexPerspectiveDivide(vertexTransform(v2, screenSpace));
	vertex maxYVert = vertexPerspectiveDivide(vertexTransform(v3, screenSpace));

	if (maxYVert.pos.y < midYVert.pos.y) {
		vertex temp = maxYVert;
		maxYVert = midYVert;
		midYVert = temp;
	}

	if (midYVert.pos.y < minYVert.pos.y) {
		vertex temp = midYVert;
		midYVert = minYVert;
		minYVert = temp;
	}

	if (maxYVert.pos.y < midYVert.pos.y) {
		vertex temp = maxYVert;
		maxYVert = midYVert;
		midYVert = temp;
	}

	scanTriangle(minYVert, midYVert, maxYVert, triangleCrossProduct(minYVert, maxYVert, midYVert) >= 0);
}
