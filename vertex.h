#ifndef VERTEX_H_
#define VERTEX_H_
#include "vector.h"
#include "math.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	vector_t pos;
	vector_t tex_coords;
} vertex_t;

static inline float triangle_cross_product(const vertex_t a, const vertex_t b, const vertex_t c) {
	return ((b.pos.x - a.pos.x) * (c.pos.y - a.pos.y) - (c.pos.x - a.pos.x) * (b.pos.y - a.pos.y));
}

static inline vertex_t vertex_perspective_divide(const vertex_t v) {
	return (vertex_t) {{v.pos.x / v.pos.w, v.pos.y / v.pos.w, v.pos.z / v.pos.w, v.pos.w}, v.tex_coords};
}

static inline vertex_t vertex_lerp(vertex_t a, vertex_t b, float lerp_factor) {
	return (vertex_t) {vector_lerp(a.pos, b.pos, lerp_factor), vector_lerp(a.tex_coords, b.tex_coords, lerp_factor)};
}

static inline bool inside_view_frustum(vertex_t vertex) {
	return _abs(vertex.pos.x) <= _abs(vertex.pos.w) &&
		_abs(vertex.pos.y) <= _abs(vertex.pos.w) &&
		_abs(vertex.pos.z) <= _abs(vertex.pos.w);
}
#endif
