#ifndef VERTEX_H_
#define VERTEX_H_
#include "vector.h"

typedef struct {
	vector pos;
	vector texCoords;
} vertex;

float triangleCrossProduct(vertex a, vertex b, vertex c);

vertex vertexPerspectiveDivide(vertex v);
#endif
