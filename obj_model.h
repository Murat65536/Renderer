#ifndef OBJ_MODEL_H_
#define OBJ_MODEL_H_
#include "list.h"
#include "indexed_model.h"

typedef struct {
	size_t vertex_index;
	size_t tex_coord_index;
	size_t normal_index;
} obj_index_t;

typedef struct {
	list positions;
	list tex_coords;
	list normals;
	list indices;
	bool has_tex_coords;
	bool has_normals;
} obj_model_t;

obj_model_t *create_obj_model(char *filename);

indexed_model_t *to_indexed_model(obj_model_t *model);

obj_index_t *parse_obj_index(obj_model_t *model, char *token);

#endif
