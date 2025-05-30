#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "vector.h"
#include "matrix.h"

typedef struct {
	float x;
	float y;
	float z;
	float w;
} quaternion_t;

quaternion_t quaternion_axis_angle(vector_t axis, float angle);

quaternion_t quaternion_matrix(matrix_t rotation);

float quaternion_length(quaternion_t quaternion);

quaternion_t quaternion_normalize(quaternion_t quaternion);

quaternion_t quaternion_conjugate(quaternion_t quaternion);

quaternion_t quaternion_multiply_float(quaternion_t quaternion, float f);

quaternion_t quaternion_multiply_quaternion(quaternion_t quaternion, quaternion_t other);

quaternion_t quaternion_multiply_vector(quaternion_t quaternion, vector_t v);

quaternion_t quaternion_subtract_quaternion(quaternion_t quaternion, quaternion_t other);

quaternion_t quaternion_add_quaternion(quaternion_t quaternion, quaternion_t other);

matrix_t quaternion_to_rotation_matrix(quaternion_t quaternion);

float quaternion_dot_product(quaternion_t quaternion, quaternion_t other);

quaternion_t quaternion_normalized_lerp(quaternion_t quaternion, quaternion_t dest, float lerp_factor, bool shortest);

quaternion_t quaternion_spherical_lerp(quaternion_t quaternion, quaternion_t dest, float lerp_factor, bool shortest);

vector_t quaternion_forward(quaternion_t quaternion);

vector_t quaternion_back(quaternion_t quaternion);

vector_t quaternion_up(quaternion_t quaternion);

vector_t quaternion_down(quaternion_t quaternion);

vector_t quaternion_right(quaternion_t quaternion);

vector_t quaternion_left(quaternion_t quaternion);

vector_t vector_rotate_quaternion(vector_t vector, quaternion_t rotation);

#endif
