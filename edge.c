#include "edge.h"
#include <stdio.h>
#include <math.h>

edge newEdge(gradients g, vertex minYVert, vertex maxYVert, int minYVertIndex) {
	int yStart = (int)ceil(minYVert.pos.y);
	int yEnd = (int)ceil(maxYVert.pos.y);

	float yDist = maxYVert.pos.y - minYVert.pos.y;
	float xDist = maxYVert.pos.x - minYVert.pos.x;

	float yPrestep = yStart - minYVert.pos.y;
	float xStep = xDist / yDist;
	float x = minYVert.pos.x + yPrestep * xStep;
	float xPrestep = x - minYVert.pos.x;
	return (edge) {
		x,
		xStep,
		yStart,
		yEnd,
		g.texCoordX[minYVertIndex] + g.texCoordXXStep * xPrestep + g.texCoordXYStep * yPrestep,
		g.texCoordXYStep + g.texCoordXXStep * xStep,
		g.texCoordY[minYVertIndex] + g.texCoordYXStep * xPrestep + g.texCoordYYStep * yPrestep,
		g.texCoordYYStep + g.texCoordYXStep * xStep,
		g.oneOverZ[minYVertIndex] + g.oneOverZXStep * xPrestep + g.oneOverZYStep * yPrestep,
		g.oneOverZYStep + g.oneOverZXStep * xStep
	};
}

void edgeStep(edge *e) {
	e->x += e->xStep;
	e->texCoordX += e->texCoordXStep;
	e->texCoordY += e->texCoordYStep;
	e->oneOverZ += e->oneOverZStep;
}
