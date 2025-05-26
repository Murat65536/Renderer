#ifndef VERTEX_H_
#define VERTEX_H_
#include "vector.h"

typedef struct {
	vector_t pos;
	vector_t tex_coords;
} vertex_t;

vertex_t *create_vertex(vector_t pos, vector_t tex_coords);

float triangle_cross_product(vertex_t a, vertex_t b, vertex_t c);

vertex_t vertex_perspective_divide(vertex_t v);
#endif
