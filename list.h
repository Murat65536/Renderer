#ifndef LIST_H_
#define LIST_H_
#include <stddef.h>

typedef struct {
	void **array;
	size_t length;
	size_t capacity;
	size_t element_size;
} list_t;

list_t *create_list(size_t element_size);

void list_add(list_t *list, void *value);

void free_list(list_t *list);

#endif
