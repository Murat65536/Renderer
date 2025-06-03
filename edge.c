#include "edge.h"
#include <stdio.h>
#include "math.h"

edge_t create_edge(gradients_t g, vertex_t min_y_vert, vertex_t max_y_vert, int min_y_vert_index) {
	int y_start = _ceil(min_y_vert.pos.y);
	int y_end = _ceil(max_y_vert.pos.y);

	float y_dist = max_y_vert.pos.y - min_y_vert.pos.y;
	float x_dist = max_y_vert.pos.x - min_y_vert.pos.x;

	float y_prestep = y_start - min_y_vert.pos.y;
	float x_step = x_dist / y_dist;
	float x = min_y_vert.pos.x + y_prestep * x_step;
	float x_prestep = x - min_y_vert.pos.x;
	return (edge_t) {
		x,
		x_step,
		y_start,
		y_end,
		g.tex_coord_x[min_y_vert_index] + g.tex_coord_xx_step * x_prestep + g.tex_coord_xy_step * y_prestep,
		g.tex_coord_xy_step + g.tex_coord_xx_step * x_step,
		g.tex_coord_y[min_y_vert_index] + g.tex_coord_yx_step * x_prestep + g.tex_coord_yy_step * y_prestep,
		g.tex_coord_yy_step + g.tex_coord_yx_step * x_step,
		g.one_over_z[min_y_vert_index] + g.one_over_zx_step * x_prestep + g.one_over_zy_step * y_prestep,
		g.one_over_zy_step + g.one_over_zx_step * x_step,
		g.depth[min_y_vert_index] + g.depth_x_step * x_prestep + g.depth_y_step * y_prestep,
		g.depth_y_step + g.depth_x_step * x_step,
    g.light_amt[min_y_vert_index] + g.light_amt_x_step * x_prestep + g.light_amt_y_step * y_prestep,
    g.light_amt_y_step + g.light_amt_x_step * x_step
	};
}

void edge_step(edge_t *e) {
	e->x += e->x_step;
	e->tex_coord_x += e->tex_coord_x_step;
	e->tex_coord_y += e->tex_coord_y_step;
	e->one_over_z += e->one_over_z_step;
	e->depth += e->depth_step;
  e->light_amt += e->light_amt_step;
}
