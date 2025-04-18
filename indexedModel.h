#ifndef INDEXED_MODEL_H_
#define INDEXED_MODEL_H_
#include "list.h"

typedef struct {
	list positions;
	list texCoords;
	list normals;
	list tangents;
	list indices;
} indexedModel;

#endif
