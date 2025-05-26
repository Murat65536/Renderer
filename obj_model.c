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
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, fp)) != -1) {
		list_t *split_list = create_list(sizeof(char*));
		char *str = strdup(line);
		char *split = "";
		while ((split = strsep(&str, " "))) {
			if (strlen(split) > 0) {
				list_add(split_list, split);
			}
		}
		if (split_list->length == 0 || (strlen((char *)split_list->array[0]) >= 1 && strcmp((char *)split_list->array[0], "#") == 0)) {
			continue;
		}
		else if (strlen((char *)split_list->array[0]) == 1 && strcmp((char *)split_list->array[0], "v") == 0) {
			vector_t *v = malloc(sizeof(vector_t));
			v->x = atof((char *)split_list->array[1]);
			v->y = atof((char *)split_list->array[2]);
			v->z = atof((char *)split_list->array[3]);
			v->w = 1.f;
			list_add(model->positions, v);
		}
		else if (strlen((char *)split_list->array[0]) == 2 && strcmp((char *)split_list->array[0], "vt") == 0) {
			vector_t *v = malloc(sizeof(vector_t));
			v->x = atof((char *)split_list->array[1]);
			v->y = 1.f - atof((char *)split_list->array[2]);
			v->z = 0.f;
			v->w = 0.f;
			list_add(model->tex_coords, v);
		}
		else if (strlen((char *)split_list->array[0]) == 2 && strcmp((char *)split_list->array[0], "vn") == 0) {
			vector_t *v = malloc(sizeof(vector_t));
			v->x = atof((char *)split_list->array[1]);
			v->y = atof((char *)split_list->array[2]);
			v->z = atof((char *)split_list->array[3]);
			v->w = 0.f;
			list_add(model->normals, v);
		}
		else if (strlen((char *)split_list->array[0]) == 1 && strcmp((char *)split_list->array[0], "f") == 0) {
			for (size_t i = 0; i < split_list->length - 3; i++) {
				list_add(model->indices, parse_obj_index(model, (char *)split_list->array[i + 1]));
				list_add(model->indices, parse_obj_index(model, (char *)split_list->array[i + 2]));
				list_add(model->indices, parse_obj_index(model, (char *)split_list->array[i + 3]));
			}
		}
	}
	fclose(fp);
	return model;
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

		vector_t *current_position = (vector_t *)model->positions->array[current_index->vertex_index];
		vector_t *current_tex_coord;
		vector_t *current_normal;

		if (model->has_tex_coords) {
			current_tex_coord = (vector_t *)model->tex_coords->array[current_index->tex_coord_index];
		}
		else {
			current_tex_coord = malloc(sizeof(*current_tex_coord));
			current_tex_coord->x = 0.f;
			current_tex_coord->y = 0.f;
			current_tex_coord->z = 0.f;
			current_tex_coord->w = 0.f;
		}

		if (model->has_normals) {
			current_normal = (vector_t *)model->normals->array[current_index->normal_index];
		}
		else {
			current_normal = malloc(sizeof(*current_normal));
			current_normal->x = 0.f;
			current_normal->y = 0.f;
			current_normal->z = 0.f;
			current_normal->w = 0.f;

		}

		size_t *model_vertex_index_ptr = (size_t *)hashmap_get(result_index_map, current_index);
		size_t model_vertex_index;
		if (model_vertex_index_ptr == NULL) {
			model_vertex_index = result->positions->length;
			hashmap_add(result_index_map, current_index, &model_vertex_index);

			list_add(result->positions, current_position);
			list_add(result->tex_coords, current_tex_coord);
			if (model->has_normals) {
				list_add(result->normals, current_normal);
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

			list_add(normal_model->positions, current_position);
			list_add(normal_model->tex_coords, current_tex_coord);
			list_add(normal_model->normals, current_normal);
			list_add(normal_model->tangents, calloc(4, sizeof(vector_t*)));
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
	list_t *split_list = create_list(sizeof(char*));
	char *str = strdup(token);
	char *split;
	while ((split = strsep(&str, "/"))) {
		if (strlen(split) > 0) {
			list_add(split_list, split);
		}
	}
	obj_index_t *result = malloc(sizeof(obj_index_t));
	result->vertex_index = atoll((char *)split_list->array[0]) - 1;

	if (split_list->length > 1) {
		if (strlen((char *)split_list->array[1]) > 0) {
			model->has_tex_coords = true;
			result->tex_coord_index = atoll((char *)split_list->array[1]) - 1;
		}
		if (split_list->length > 2) {
			model->has_normals = true;
			result->normal_index = atoll((char *)split_list->array[2]) - 1;
		}
	}

	return result;
}
