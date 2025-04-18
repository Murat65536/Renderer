#ifndef DISPLAY_H_
#define DISPLAY_H_
#include "vertex.h"
#include "edge.h"
#include "matrix.h"
#include <stdbool.h>

void resetTerminalMode();

void queueResUpdate();

void resUpdate();

void initDisplay();

matrix getProjectionMatrix();

bool kbhit();

int getch();

void render();

void clear();

unsigned int getRGB(unsigned char r, unsigned char g, unsigned char b);

void plotPoint(unsigned int x, unsigned int y, unsigned int color);

void drawScanLine(edge *left, edge *right, int j);

void scanEdges(edge *a, edge *b, bool side);

void scanTriangle(vertex minYVert, vertex midYVert, vertex maxYVert, bool side);

void fillTriangle(vertex v1, vertex v2, vertex v3);

#endif
