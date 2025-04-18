#include "list.h"
#include <stdlib.h>

list newList() {
	return (list) {malloc(sizeof(void*)), 0, 1};
}

void listAdd(list *l, void *value) {
	l->capacity *= 2;
	l = realloc(l, l->capacity * sizeof(void*));
	l->array[l->length++] = value;
}

void freeList(list *l) {
	for (unsigned int i = 0; i < l->length; i++) {
		free(l->array[i]);
	}
	free(l->array);
}
