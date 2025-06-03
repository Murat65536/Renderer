#include <stdlib.h>
#include <stdio.h>
#include "indexed_model.h"
#include "vector.h"

indexed_model_t *create_indexed_model() {
	indexed_model_t *indexed_model;
	indexed_model = malloc(sizeof(*indexed_model));
	indexed_model->positions = create_list(sizeof(vector_t));
	indexed_model->tex_coords = create_list(sizeof(vector_t));
	indexed_model->normals = create_list(sizeof(vector_t));
	indexed_model->tangents = create_list(sizeof(vector_t));
	indexed_model->indices = create_list(sizeof(size_t));
	return indexed_model;
}

void free_indexed_model(indexed_model_t *indexed_model) {
	free_list(indexed_model->positions);
	free_list(indexed_model->tex_coords);
	free_list(indexed_model->normals);
	free_list(indexed_model->tangents);
	free_list(indexed_model->indices);
	free(indexed_model);
}

void calc_normals(indexed_model_t *indexed_model) {
	for (size_t i = 0; i < indexed_model->indices->length; i += 3) {
		size_t i0 = *(size_t *)indexed_model->indices->array[i];
		size_t i1 = *(size_t *)indexed_model->indices->array[i + 1];
		size_t i2 = *(size_t *)indexed_model->indices->array[i + 2];
		vector_t v1 = vector_subtract_vector(*(vector_t *)indexed_model->positions->array[i1], *(vector_t *)indexed_model->positions->array[i0]);
		vector_t v2 = vector_subtract_vector(*(vector_t *)indexed_model->positions->array[i2], *(vector_t *)indexed_model->positions->array[i0]);

		vector_t normal = vector_normalize(vector_cross_product(v1, v2));

		vector_t nv1 = vector_add_vector(*(vector_t *)indexed_model->normals->array[i0], normal);
		vector_t nv2 = vector_add_vector(*(vector_t *)indexed_model->normals->array[i1], normal);
		vector_t nv3 = vector_add_vector(*(vector_t *)indexed_model->normals->array[i2], normal);

		indexed_model->normals->array[i0] = &nv1;
		indexed_model->normals->array[i1] = &nv2;
		indexed_model->normals->array[i2] = &nv3;
	}
	for (size_t i = 0; i < indexed_model->indices->length; i++) {
		vector_t v = vector_normalize(*(vector_t *)indexed_model->normals->array[i]);
		indexed_model->normals->array[i] = &v;
	}
}

void calc_tangents(indexed_model_t *indexed_model) {
	for (size_t i = 0; i < indexed_model->indices->length; i += 3) {
		size_t i0 = *(size_t *)indexed_model->indices->array[i];
		size_t i1 = *(size_t *)indexed_model->indices->array[i + 1];
		size_t i2 = *(size_t *)indexed_model->indices->array[i + 2];

		vector_t edge1 = vector_subtract_vector(*(vector_t *)indexed_model->positions->array[i1], *(vector_t *)indexed_model->positions->array[i0]);
		vector_t edge2 = vector_subtract_vector(*(vector_t *)indexed_model->positions->array[i2], *(vector_t *)indexed_model->positions->array[i0]);

		float deltaU1 = ((vector_t *)indexed_model->tex_coords->array[i1])->x - ((vector_t *)indexed_model->tex_coords->array[i0])->x;
		float deltaV1 = ((vector_t *)indexed_model->tex_coords->array[i1])->y - ((vector_t *)indexed_model->tex_coords->array[i0])->y;
		float deltaU2 = ((vector_t *)indexed_model->tex_coords->array[i2])->x - ((vector_t *)indexed_model->tex_coords->array[i0])->x;
		float deltaV2 = ((vector_t *)indexed_model->tex_coords->array[i2])->y - ((vector_t *)indexed_model->tex_coords->array[i0])->y;

		float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
		float f;
		if (dividend == 0.f) {
			f = 0.f;
		}
		else {
			f = 1.f / dividend;
		}

		vector_t tangent = {
			f * (deltaV2 * edge1.x - deltaV1 * edge2.x),
			f * (deltaV2 * edge1.y - deltaV1 * edge2.y),
			f * (deltaV2 * edge1.z - deltaV1 * edge2.z),
			0.f
		};
		vector_t v1 = vector_add_vector(*(vector_t *)indexed_model->tangents->array[i0], tangent);
		vector_t v2 = vector_add_vector(*(vector_t *)indexed_model->tangents->array[i1], tangent);
		vector_t v3 = vector_add_vector(*(vector_t *)indexed_model->tangents->array[i2], tangent);

		indexed_model->tangents->array[i0] = &v1;
		indexed_model->tangents->array[i1] = &v2;
		indexed_model->tangents->array[i2] = &v3;
	}
	for (size_t i = 0; i < indexed_model->tangents->length; i++) {
		vector_t v = vector_normalize(*(vector_t *)indexed_model->tangents->array[i]);
		indexed_model->tangents->array[i] = &v;
	}
}
