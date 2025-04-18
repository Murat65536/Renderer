#ifndef GRADIENTS_H_
#define GRADIENTS_H_
#include "vertex.h"

typedef struct {
	float texCoordX[3];
	float texCoordY[3];
	float oneOverZ[3];
	float texCoordXXStep;
	float texCoordXYStep;
	float texCoordYXStep;
	float texCoordYYStep;
	float oneOverZXStep;
	float oneOverZYStep;
} gradients;

float gradientCalcXStep(float *values, vertex minYVert, vertex midYVert, vertex maxYVert, float oneOverDX);

float gradientCalcYStep(float *values, vertex minYVert, vertex midYVert, vertex maxYvert, float oneOverDY);

gradients newGradients(vertex minYvert, vertex midYvert, vertex maxYVert);

#endif
