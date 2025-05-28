#include "vertex.h"
#include <stdlib.h>
#include <math.h>

vertex_t *malloc_vertex(vertex_t vertex) {
	vertex_t *v;
	v = malloc(sizeof(*v));
	v->pos = vertex.pos;
	v->tex_coords = vertex.tex_coords;
	return v;
}

vertex_t *create_vertex(vector_t pos, vector_t tex_coords) {
	vertex_t *vertex = malloc(sizeof(vertex_t));
	vertex->pos = (vector_t) {pos.x, pos.y, pos.z, pos.w};
	vertex->tex_coords = (vector_t) {tex_coords.x, tex_coords.y, tex_coords.z, tex_coords.w};
	return vertex;
}

float triangle_cross_product(vertex_t a, vertex_t b, vertex_t c) {
	const float x1 = b.pos.x - a.pos.x;
	const float y1 = b.pos.y - a.pos.y;

	const float x2 = c.pos.x - a.pos.x;
	const float y2 = c.pos.y - a.pos.y;

	return (x1 * y2 - x2 * y1);
}

vertex_t vertex_perspective_divide(vertex_t v) {
	return (vertex_t) {{v.pos.x / v.pos.w, v.pos.y / v.pos.w, v.pos.z / v.pos.w, v.pos.w}, v.tex_coords};
}

vertex_t vertex_lerp(vertex_t a, vertex_t b, float lerp_factor) {
	return (vertex_t) {vector_lerp(a.pos, b.pos, lerp_factor), vector_lerp(a.tex_coords, b.tex_coords, lerp_factor)};
}

bool inside_view_frustum(vertex_t vertex) {
	return fabsf(vertex.pos.x) <= fabsf(vertex.pos.w) &&
		fabsf(vertex.pos.y) <= fabsf(vertex.pos.w) &&
		fabsf(vertex.pos.z) <= fabsf(vertex.pos.w);
}
