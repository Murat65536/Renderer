#include "mesh.h"
#include "list.h"
#include "indexed_model.h"
#include "obj_model.h"
#include "vertex.h"
#include <stddef.h>
#include <stdlib.h>

mesh_t *create_mesh(char *filename) {
	mesh_t *mesh = malloc(sizeof(mesh_t));
	mesh->vertices = create_list(sizeof(vertex_t));
	indexed_model_t *model = to_indexed_model(create_obj_model(filename));
	for (size_t i = 0; i < model->positions->length; i++) {
		list_add(mesh->vertices, create_vertex(*(vector_t *)model->positions->array[i], *(vector_t *)model->tex_coords->array[i]));
	}
	mesh->indices = model->indices;
	return mesh;
}
