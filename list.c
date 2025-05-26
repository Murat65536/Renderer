#include "list.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 16

list_t *create_list(size_t element_size) {
	list_t *list;
	list = malloc(sizeof(*list));
	list->array = malloc(sizeof(void*) * INITIAL_SIZE);
	list->length = 0;
	list->capacity = INITIAL_SIZE;
	list->element_size = element_size;
	return list;
}

void list_add(list_t *list, void *value) {
	if (list->length >= list->capacity) {
		list->capacity *= 2;
		list->array = realloc(list->array, list->capacity * sizeof(void*));
	}
	list->array[list->length] = malloc(list->element_size);
	memcpy(list->array[list->length++], value, list->element_size);
}

void free_list(list_t *list) {
	for (unsigned int i = 0; i < list->length; i++) {
		free(list->array[i]);
	}
	free(list->array);
}
