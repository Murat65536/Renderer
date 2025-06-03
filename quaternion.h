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

quaternion_t quaternion_axis_angle(const vector_t axis, const float angle);

quaternion_t quaternion_matrix(const matrix_t rotation);

float quaternion_length(const quaternion_t quaternion);

quaternion_t quaternion_normalize(const quaternion_t quaternion);

quaternion_t quaternion_conjugate(const quaternion_t quaternion);

quaternion_t quaternion_multiply_float(const quaternion_t quaternion, const float f);

quaternion_t quaternion_multiply_quaternion(const quaternion_t quaternion, const quaternion_t other);

quaternion_t quaternion_multiply_vector(const quaternion_t quaternion, const vector_t v);

quaternion_t quaternion_subtract_quaternion(const quaternion_t quaternion, const quaternion_t other);

quaternion_t quaternion_add_quaternion(const quaternion_t quaternion, const quaternion_t other);

matrix_t quaternion_to_rotation_matrix(const quaternion_t quaternion);

float quaternion_dot_product(const quaternion_t quaternion, const quaternion_t other);

quaternion_t quaternion_normalized_lerp(const quaternion_t quaternion, quaternion_t dest, const float lerp_factor, const bool shortest);

quaternion_t quaternion_spherical_lerp(const quaternion_t quaternion, quaternion_t dest, const float lerp_factor, const bool shortest);

vector_t quaternion_forward(const quaternion_t quaternion);

vector_t quaternion_back(const quaternion_t quaternion);

vector_t quaternion_up(const quaternion_t quaternion);

vector_t quaternion_down(const quaternion_t quaternion);

vector_t quaternion_right(const quaternion_t quaternion);

vector_t quaternion_left(const quaternion_t quaternion);

vector_t vector_rotate_quaternion(const vector_t vector, const quaternion_t rotation);

#endif
