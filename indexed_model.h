#ifndef INDEXED_MODEL_H_
#define INDEXED_MODEL_H_
#include "list.h"

typedef struct {
	list_t *positions;
	list_t *tex_coords;
	list_t *normals;
	list_t *tangents;
	list_t *indices;
} indexed_model_t;

indexed_model_t *create_indexed_model();

void free_indexed_model(indexed_model_t *indexed_model);

void calc_normals(indexed_model_t *indexed_model);

void calc_tangents(indexed_model_t *indexed_model);

#endif
