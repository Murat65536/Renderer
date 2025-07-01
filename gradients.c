#include "gradients.h"
#include "math.h"
#include <stdio.h>

float gradient_calc_x_step(float *values, vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, float one_over_dx) {
	return (((values[1] - values[2]) * (min_y_vert.pos.y - max_y_vert.pos.y)) - ((values[0] - values[2]) * (mid_y_vert.pos.y - max_y_vert.pos.y))) * one_over_dx;
}

float gradient_calc_y_step(float *values, vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, float one_over_dy) {
	return (((values[1] - values[2]) * (min_y_vert.pos.x - max_y_vert.pos.x)) - ((values[0] - values[2]) * (mid_y_vert.pos.x - max_y_vert.pos.x))) * one_over_dy;
}

gradients_t create_gradient(vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert) {	
	float one_over_dx = 1.f / (((mid_y_vert.pos.x - max_y_vert.pos.x) *
							  (min_y_vert.pos.y - max_y_vert.pos.y)) -
							 ((min_y_vert.pos.x - max_y_vert.pos.x) *
							  (mid_y_vert.pos.y - max_y_vert.pos.y)));
	float one_over_dy = -one_over_dx;

	gradients_t g = {0};
	g.one_over_z[0] = 1.f / min_y_vert.pos.w;
	g.one_over_z[1] = 1.f / mid_y_vert.pos.w;
	g.one_over_z[2] = 1.f / max_y_vert.pos.w;
	g.tex_coord_x[0] = min_y_vert.tex_coords.x * g.one_over_z[0];
	g.tex_coord_x[1] = mid_y_vert.tex_coords.x * g.one_over_z[1];
	g.tex_coord_x[2] = max_y_vert.tex_coords.x * g.one_over_z[2];
	g.tex_coord_y[0] = min_y_vert.tex_coords.y * g.one_over_z[0];
	g.tex_coord_y[1] = mid_y_vert.tex_coords.y * g.one_over_z[1];
	g.tex_coord_y[2] = max_y_vert.tex_coords.y * g.one_over_z[2];
	g.depth[0] = min_y_vert.pos.z;
	g.depth[1] = mid_y_vert.pos.z;
	g.depth[2] = max_y_vert.pos.z;
	vector_t light_dir = (vector_t) {0.f, 0.f, 1.f, 1.f};
	g.light_amt[0] = _clamp(vector_dot_product(min_y_vert.normal, light_dir), 0.f, 1.f) * 0.9f + 0.1f;
	g.light_amt[1] = _clamp(vector_dot_product(mid_y_vert.normal, light_dir), 0.f, 1.f) * 0.9f + 0.1f;
	g.light_amt[2] = _clamp(vector_dot_product(max_y_vert.normal, light_dir), 0.f, 1.f) * 0.9f + 0.1f;
	g.tex_coord_xx_step = gradient_calc_x_step(g.tex_coord_x, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
	g.tex_coord_xy_step = gradient_calc_y_step(g.tex_coord_x, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
	g.tex_coord_yx_step = gradient_calc_x_step(g.tex_coord_y, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
	g.tex_coord_yy_step = gradient_calc_y_step(g.tex_coord_y, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
	g.one_over_zx_step = gradient_calc_x_step(g.one_over_z, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
	g.one_over_zy_step = gradient_calc_y_step(g.one_over_z, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
	g.depth_x_step = gradient_calc_x_step(g.depth, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
	g.depth_y_step = gradient_calc_y_step(g.depth, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
	g.light_amt_x_step = gradient_calc_x_step(g.light_amt, min_y_vert, mid_y_vert, max_y_vert, one_over_dx);
	g.light_amt_y_step = gradient_calc_y_step(g.light_amt, min_y_vert, mid_y_vert, max_y_vert, one_over_dy);
	return g;
}
