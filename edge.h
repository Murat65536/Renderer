#ifndef EDGE_H_
#define EDGE_H_
#include "gradients.h"
#include "vertex.h"

typedef struct {
	float x;
	float x_step;
	int y_start;
	int y_end;
	float tex_coord_x;
	float tex_coord_x_step;
	float tex_coord_y;
	float tex_coord_y_step;
	float one_over_z;
	float one_over_z_step;
} edge_t;

edge_t create_edge(gradients_t g, vertex_t min_y_vert, vertex_t max_y_vert, int min_y_vert_index);

void edge_step(edge_t *e);

#endif
