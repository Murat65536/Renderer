#include "transform.h"
#include "quaternion.h"
#include "vector.h"
#include "matrix.h"
#include "vector.h"

transform_t transform_pos(vector_t pos) {
  return (transform_t) {pos, {0.f, 0.f, 0.f, 1.f}, {1.f, 1.f, 1.f, 1.f}};
}

transform_t transform_rotate(transform_t transform, quaternion_t rotation) {
	return (transform_t) {transform.pos, quaternion_normalize(quaternion_multiply_quaternion(transform.rot, rotation)), transform.scale};
}

transform_t transform_look_at(transform_t transform, vector_t point, vector_t up) {
	return transform_rotate(transform, transform_get_look_at_rotation(transform, point, up));
}

quaternion_t transform_get_look_at_rotation(transform_t transform, vector_t point, vector_t up) {
	return quaternion_matrix(init_matrix_rotation_forward_up(vector_normalize(vector_subtract_vector(point, transform.pos)), up));
}

matrix_t get_transformation(transform_t transform) {
	matrix_t translation_matrix = init_matrix_translation(transform.pos.x, transform.pos.y, transform.pos.z);
	matrix_t rotation_matrix = quaternion_to_rotation_matrix(transform.rot);
	matrix_t scale_matrix = init_matrix_scale(transform.scale.x, transform.scale.y, transform.scale.z);

	return multiply_matrices(translation_matrix, multiply_matrices(rotation_matrix, scale_matrix));
}
