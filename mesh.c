#include "mesh.h"
#include "list.h"
#include "indexed_model.h"
#include "obj_model.h"
#include "vertex.h"
#include <stddef.h>
#include <stdlib.h>

mesh_t *create_mesh(char *filename) {
	mesh_t *mesh = malloc(sizeof(mesh_t));
	mesh->indices = create_list(sizeof(vertex_t));
	mesh->vertices = create_list(sizeof(vertex_t));
	obj_model_t *obj_model = create_obj_model(filename);
	indexed_model_t *indexed_model = to_indexed_model(obj_model);
	free_obj_model(obj_model);
	for (size_t i = 0; i < indexed_model->positions->length; i++) {
    vertex_t v = (vertex_t) {*(vector_t *)indexed_model->positions->array[i], *(vector_t *)indexed_model->tex_coords->array[i]};
		list_add(mesh->vertices, &v);
	}
	for (size_t i = 0; i < indexed_model->indices->length; i++) {
		list_add(mesh->indices, indexed_model->indices->array[i]);
	}
	free_indexed_model(indexed_model);
	return mesh;
}

void free_mesh(mesh_t *mesh) {
	free_list(mesh->indices);
	free_list(mesh->vertices);
	free(mesh);
}
