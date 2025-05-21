#ifndef LIST_H_
#define LIST_H_
#include <stddef.h>

typedef struct {
	void **array;
	size_t length;
	size_t capacity;
} list;

list newList();

void listAdd(list *l, void *value);

#endif
