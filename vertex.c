#include "vertex.h"

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
