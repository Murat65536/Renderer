#ifndef LIST_H_
#define LIST_H_

typedef struct {
	void **array;
	unsigned int length;
	unsigned int capacity;
} list;

list newList();

void listAdd(list *l, void *value);

#endif
