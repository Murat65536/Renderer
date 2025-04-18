#ifndef EDGE_H_
#define EDGE_H_
#include "gradients.h"
#include "vertex.h"

typedef struct {
	float x;
	float xStep;
	int yStart;
	int yEnd;
	float texCoordX;
	float texCoordXStep;
	float texCoordY;
	float texCoordYStep;
	float oneOverZ;
	float oneOverZStep;
} edge;

edge newEdge(gradients g, vertex minYVert, vertex maxYVert, int minYVertIndex);

void edgeStep(edge *e);

#endif
