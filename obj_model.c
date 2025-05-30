#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "obj_model.h"
#include "list.h"
#include "vector.h"
#include "hashmap.h"

obj_model_t *create_obj_model(char *filename) {
	obj_model_t *model = malloc(sizeof(obj_model_t));
	model->positions = create_list(sizeof(vector_t));
	model->tex_coords = create_list(sizeof(vector_t));
	model->normals = create_list(sizeof(vector_t));
	model->indices = create_list(sizeof(obj_index_t));
	model->has_tex_coords = false;
	model->has_normals = false;

	FILE *fp = fopen(filename, "r");
	char line[256];

	while (fgets(line, sizeof(line), fp)) {
		char *split_list[32];
		char *str = strdup(line);
		char *split = "";
		size_t index = 0;
		while ((split = strsep(&str, " ")) && index < 32) {
			if (strlen(split) > 0) {
				split_list[index++] = split;
			}
		}
		if (index == 0 || (strlen(split_list[0]) >= 1 && strcmp(split_list[0], "#") == 0)) {
			continue;
		}
		else if (strlen(split_list[0]) == 1 && strcmp(split_list[0], "v") == 0) {
			vector_t v = {atof(split_list[1]), atof(split_list[2]), atof(split_list[3]), 1.f};
			list_add(model->positions, &v);
		}
		else if (strlen(split_list[0]) == 2 && strcmp(split_list[0], "vt") == 0) {
			vector_t v = {atof(split_list[1]), 1.f - atof(split_list[2]), 0.f, 0.f};
			list_add(model->tex_coords, &v);
		}
		else if (strlen(split_list[0]) == 2 && strcmp(split_list[0], "vn") == 0) {
			vector_t v = (vector_t) {atof(split_list[1]), atof(split_list[2]), atof(split_list[3]), 0.f};
			list_add(model->normals, &v);
		}
		else if (strlen(split_list[0]) == 1 && strcmp(split_list[0], "f") == 0) {
			for (size_t i = 0; i < index - 3; i++) {
				list_add(model->indices, parse_obj_index(model, split_list[1]));
				list_add(model->indices, parse_obj_index(model, split_list[i + 2]));
				list_add(model->indices, parse_obj_index(model, split_list[i + 3]));
			}
		}
	}
	fclose(fp);
	return model;
}

void free_obj_model(obj_model_t *obj_model) {
	free_list(obj_model->positions);
	free_list(obj_model->tex_coords);
	free_list(obj_model->normals);
	free_list(obj_model->indices);
	free(obj_model);
}

bool compare_size_t(void *s1, void *s2) {
	return *(size_t *)s1 == *(size_t *)s2;
}

bool compare_obj_index_t(void *o1, void *o2) {
	return ((obj_index_t*)o1)->vertex_index == ((obj_index_t*)o2)->vertex_index && ((obj_index_t*)o1)->tex_coord_index == ((obj_index_t*)o2)->tex_coord_index && ((obj_index_t*)o1)->normal_index == ((obj_index_t*)o2)->normal_index;
}

indexed_model_t *to_indexed_model(obj_model_t *model) {
	indexed_model_t *result = create_indexed_model();
	indexed_model_t *normal_model = create_indexed_model();
	hashmap_t *result_index_map = create_hashmap(sizeof(obj_index_t), sizeof(size_t), compare_obj_index_t);
	hashmap_t *normal_index_map = create_hashmap(sizeof(size_t), sizeof(size_t), compare_size_t);
	hashmap_t *index_map = create_hashmap(sizeof(size_t), sizeof(size_t), compare_size_t);
	for (size_t i = 0; i < model->indices->length; i++) {
		obj_index_t *current_index = (obj_index_t *)model->indices->array[i];

		vector_t current_position = *(vector_t *)model->positions->array[current_index->vertex_index];
		vector_t current_tex_coord;
		vector_t current_normal;

		if (model->has_tex_coords) {
			current_tex_coord = *(vector_t *)model->tex_coords->array[current_index->tex_coord_index];
		}
		else {
			current_tex_coord = (vector_t) {0.f, 0.f, 0.f, 0.f};
		}

		if (model->has_normals) {
			current_normal = *(vector_t *)model->normals->array[current_index->normal_index];
		}
		else {
			current_normal = (vector_t) {0.f, 0.f, 0.f, 0.f};

		}

		size_t *model_vertex_index_ptr = (size_t *)hashmap_get(result_index_map, current_index);
		size_t model_vertex_index;
		if (model_vertex_index_ptr == NULL) {
			model_vertex_index = result->positions->length;
			hashmap_add(result_index_map, current_index, &model_vertex_index);

			list_add(result->positions, &current_position);
			list_add(result->tex_coords, &current_tex_coord);
			if (model->has_normals) {
				list_add(result->normals, &current_normal);
			}
		}
		else {
			model_vertex_index = *model_vertex_index_ptr;
		}

		size_t *normal_model_index_ptr = (size_t *)hashmap_get(normal_index_map, &current_index->vertex_index);
		size_t normal_model_index;
		if (normal_model_index_ptr == NULL) {
			normal_model_index = normal_model->positions->length;
			hashmap_add(normal_index_map, &current_index->vertex_index, &normal_model_index);

			list_add(normal_model->positions, &current_position);
			list_add(normal_model->tex_coords, &current_tex_coord);
			list_add(normal_model->normals, &current_normal);
			vector_t v = {0.f, 0.f, 0.f, 0.f};
			list_add(normal_model->tangents, &v);
		}
		else {
			normal_model_index = *normal_model_index_ptr;
		}
		list_add(result->indices, &model_vertex_index);
		list_add(normal_model->indices, &normal_model_index);
		hashmap_add(index_map, &model_vertex_index, &normal_model_index);
	}

	if (!model->has_normals) {
		calc_normals(normal_model);
		for (size_t i = 0; i < result->positions->length; i++) {
			list_add(result->normals, normal_model->normals->array[*(size_t *)hashmap_get(index_map, &i)]);
		}
	}
	calc_tangents(normal_model);

	for (size_t i = 0; i < result->positions->length; i++) {
		list_add(result->tangents, normal_model->tangents->array[*(size_t *)hashmap_get(index_map, &i)]);
	}
	return result;
}

obj_index_t *parse_obj_index(obj_model_t *model, char *token) {
	char *split_list[3];
	char *str = strdup(token);
	char *split = "";
	int i = -1;
	while ((split = strsep(&str, "/")) && i < 2) {
		if (strlen(split) > 0) {
			split_list[++i] = split;
		}
	}
	obj_index_t *result = malloc(sizeof(obj_index_t));
	result->vertex_index = atoll(split_list[0]) - 1;

	if (i > 0) {
		if (strlen(split_list[i]) > 0) {
			model->has_tex_coords = true;
			result->tex_coord_index = atoll(split_list[1]) - 1;
		}
		if (i > 1) {
			model->has_normals = true;
			result->normal_index = atoll(split_list[2]) - 1;
		}
	}

	return result;
}
