#include "camera.h"
#include "matrix.h"
#include "vector.h"
#include "quaternion.h"
#include "transform.h"

matrix_t get_view_projection(camera_t *camera) {
	matrix_t camera_rotation = quaternion_to_rotation_matrix(quaternion_conjugate(camera->transform.rot));
	vector_t camera_pos = vector_multiply_float(camera->transform.pos, -1.f);

	matrix_t camera_translation = init_matrix_translation(camera_pos.x, camera_pos.y, camera_pos.z);

	return multiply_matrices(camera->projection, multiply_matrices(camera_rotation, camera_translation));
}

void camera_move(camera_t *camera, vector_t dir, float amount) {
	camera->transform.pos = vector_add_vector(camera->transform.pos, vector_multiply_float(dir, amount));
}

void camera_rotate(camera_t *camera, vector_t axis, float angle) {
	camera->transform = transform_rotate(camera->transform, quaternion_axis_angle(axis, angle));
}
