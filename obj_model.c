#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "obj_model.h"
#include "list.h"
#include "vector.h"

obj_model_t *create_obj_model(char *filename) {
	obj_model_t *model;
	model = malloc(sizeof(*model));
	model->positions = newList();
	model->tex_coords = newList();
	model->normals = newList();
	model->indices = newList();
	model->has_tex_coords = false;
	model->has_normals = false;

	FILE *fp = fopen(filename, "r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	while ((read = getline(&line, &len, fp)) != -1) {
		list split_list = newList();
		char *s = strtok(line, " ");
		while (s != NULL) {
			if (strcmp(s, "") != 0) {
				listAdd(&split_list, s);
			}
			s = strtok(NULL, " ");
		}
		if (strcmp(line, "") == 0 || strcmp((char *)split_list.array[0], "#") == 0) {
			continue;
		}
		else if (strcmp((char *)split_list.array[0], "v") == 0) {
			vector_t *v;
			v = malloc(sizeof(*v));
			v->x = atof((char *)split_list.array[1]);
			v->y = atof((char *)split_list.array[2]);
			v->z = atof((char *)split_list.array[3]);
			v->w = 1.f;
			listAdd(&model->positions, v);
		}
		else if (strcmp((char *)split_list.array[0], "vt") == 0) {
			vector_t *v;
			v = malloc(sizeof(*v));
			v->x = atof((char *)split_list.array[1]);
			v->y = 1.f - atof((char *)split_list.array[1]);
			v->z = 0.f;
			v->w = 0.f;
			listAdd(&model->tex_coords, v);
		}
		else if (strcmp((char *)split_list.array[0], "f") == 0) {
			for (size_t i = 0; i < split_list.length - 3; i++) {
				listAdd(&model->indices, parse_obj_index(model, (char *)split_list.array[1]));
				listAdd(&model->indices, parse_obj_index(model, (char *)split_list.array[i + 2]));
				listAdd(&model->indices, parse_obj_index(model, (char *)split_list.array[i + 3]));
			}
		}
	}
	fclose(fp);
	return model;
}

indexed_model_t *to_indexed_model(obj_model_t *model) {
	
}

obj_index_t *parse_obj_index(obj_model_t *model, char *token) {
	list split_list = newList();
	char *s = strtok(token, "/");
	while (token != NULL) {
		if (strcmp(s, "") != 0) {
			listAdd(&split_list, s);
		}
		s = strtok(NULL, "/");
	}
	obj_index_t *result;
	result = malloc(sizeof(*result));
	result->vertex_index = atoll((char *)split_list.array[0]) - 1;

	if (split_list.length > 1) {
		if (strcmp((char *)split_list.array[1], "") != 0) {
			model->has_tex_coords = true;
			result->tex_coord_index = atoll((char *)split_list.array[1]) - 1;
		}
		if (split_list.length > 2) {
			model->has_normals = true;
			result->normal_index = atoll((char *)split_list.array[2]) - 1;
		}
	}

	return result;
}
