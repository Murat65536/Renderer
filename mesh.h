#ifndef MESH_H_
#define MESH_H_
#include "list.h"
#include "bitmap.h"
#include "matrix.h"

typedef struct {
	list_t *vertices;
	list_t *indices;
  matrix_t projection;
  bitmap_t *texture;
} mesh_t;

mesh_t *create_mesh(char *model_filename, bitmap_t *texture, matrix_t projection);

void free_mesh(mesh_t *mesh);

#endif
