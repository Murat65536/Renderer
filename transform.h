#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "quaternion.h"
#include "vector.h"
#include "matrix.h"

typedef struct {
	vector_t pos;
	quaternion_t rot;
	vector_t scale;
} transform_t;

transform_t transform_pos(vector_t pos);

transform_t transform_rotate(transform_t transform, quaternion_t rotation);

transform_t transform_look_at(transform_t transform, vector_t point, vector_t up);

quaternion_t transform_get_look_at_rotation(transform_t transform, vector_t point, vector_t up);

matrix_t get_transformation(transform_t transform);

#endif
