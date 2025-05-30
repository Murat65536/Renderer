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
	list_t *positions;
	list_t *tex_coords;
	list_t *normals;
	list_t *indices;
	bool has_tex_coords;
	bool has_normals;
} obj_model_t;

obj_model_t *create_obj_model(char *filename);

void free_obj_model(obj_model_t *obj_model);

bool compare_size_t(void *s1, void *s2);

bool compare_obj_index_t(void *o1, void *o2);

indexed_model_t *to_indexed_model(obj_model_t *model);

obj_index_t *parse_obj_index(obj_model_t *model, char *token);

#endif
