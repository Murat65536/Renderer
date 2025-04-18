#include "gradients.h"
#include <stdio.h>

float gradientCalcXStep(float *values, vertex minYVert, vertex midYVert, vertex maxYVert, float oneOverDX) {
	return (((values[1] - values[2]) * (minYVert.pos.y - maxYVert.pos.y)) - ((values[0] - values[2]) * (midYVert.pos.y - maxYVert.pos.y))) * oneOverDX;
}

float gradientCalcYStep(float *values, vertex minYVert, vertex midYVert, vertex maxYVert, float oneOverDY) {
	return (((values[1] - values[2]) * (minYVert.pos.x - maxYVert.pos.x)) - ((values[0] - values[2]) * (midYVert.pos.x - maxYVert.pos.x))) * oneOverDY;
}

gradients newGradients(vertex minYVert, vertex midYVert, vertex maxYVert) {	
	float oneOverDX = 1.f / (((midYVert.pos.x - maxYVert.pos.x) *
							  (minYVert.pos.y - maxYVert.pos.y)) -
							 ((minYVert.pos.x - maxYVert.pos.x) *
							  (midYVert.pos.y - maxYVert.pos.y)));

	float oneOverDY = -oneOverDX;

	gradients g = {0};
	g.oneOverZ[0] = 1.f / minYVert.pos.w;
	g.oneOverZ[1] = 1.f / midYVert.pos.w;
	g.oneOverZ[2] = 1.f / maxYVert.pos.w;
	g.texCoordX[0] = minYVert.texCoords.x * g.oneOverZ[0];
	g.texCoordX[1] = midYVert.texCoords.x * g.oneOverZ[1];
	g.texCoordX[2] = maxYVert.texCoords.x * g.oneOverZ[2];
	g.texCoordY[0] = minYVert.texCoords.y * g.oneOverZ[0];
	g.texCoordY[1] = midYVert.texCoords.y * g.oneOverZ[1];
	g.texCoordY[2] = maxYVert.texCoords.y * g.oneOverZ[2];
	g.texCoordXXStep = gradientCalcXStep(g.texCoordX, minYVert, midYVert, maxYVert, oneOverDX);
	g.texCoordXYStep = gradientCalcYStep(g.texCoordX, minYVert, midYVert, maxYVert, oneOverDY);
	g.texCoordYXStep = gradientCalcXStep(g.texCoordY, minYVert, midYVert, maxYVert, oneOverDX);
	g.texCoordYYStep = gradientCalcYStep(g.texCoordY, minYVert, midYVert, maxYVert, oneOverDY);
	g.oneOverZXStep = gradientCalcXStep(g.oneOverZ, minYVert, midYVert, maxYVert, oneOverDX);
	g.oneOverZYStep = gradientCalcYStep(g.oneOverZ, minYVert, midYVert, maxYVert, oneOverDY);
	return g;
}
