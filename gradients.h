#ifndef GRADIENTS_H_
#define GRADIENTS_H_
#include "vertex.h"

typedef struct {
	float tex_coord_x[3];
	float tex_coord_y[3];
	float one_over_z[3];
	float depth[3];
  float light_amt[3];
	float tex_coord_xx_step;
	float tex_coord_xy_step;
	float tex_coord_yx_step;
	float tex_coord_yy_step;
	float one_over_zx_step;
	float one_over_zy_step;
	float depth_x_step;
	float depth_y_step;
  float light_amt_x_step;
  float light_amt_y_step;
} gradients_t;

float gradient_calc_x_step(float *values, vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, float one_over_dx);

float gradient_calc_y_step(float *values, vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert, float one_over_dy);

gradients_t create_gradient(vertex_t min_y_vert, vertex_t mid_y_vert, vertex_t max_y_vert);

#endif
