#ifndef MESH_H_
#define MESH_H_
#include "list.h"

typedef struct {
	list_t *vertices;
	list_t *indices;
} mesh_t;

mesh_t *create_mesh(char *filename);

#endif
