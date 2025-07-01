#include "camera.h"
#include "matrix.h"
#include "vector.h"
#include "input.h"
#include "quaternion.h"
#include "transform.h"

#define MOVEMENT_SPEED 4.f
#define SENSITIVITY 1.5f

camera_t *create_camera() {
  camera_t *camera = malloc(sizeof(camera_t));
	camera->transform = (transform_t) {{0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}, {1.f, 1.f, 1.f, 1.f}};
  return camera;
}

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

int camera_update(camera_t *camera, const float delta_time) {
	const vector_t Y_AXIS = (vector_t) {0.f, 1.f, 0.f, 1.f};
	const vector_t X_AXIS = (vector_t) {1.f, 0.f, 0.f, 1.f};
  if (pressed[16]) {
		return -1;
  }
  if (pressed[17]) {
    camera_move(camera, quaternion_forward(camera->transform.rot), MOVEMENT_SPEED * delta_time);
  }
  if (pressed[30]) {
    camera_move(camera, quaternion_left(camera->transform.rot), MOVEMENT_SPEED * delta_time);
  }
  if (pressed[31]) {
    camera_move(camera, quaternion_back(camera->transform.rot), MOVEMENT_SPEED * delta_time);
  }
  if (pressed[32]) {
    camera_move(camera, quaternion_right(camera->transform.rot), MOVEMENT_SPEED * delta_time);
  }
  if (pressed[57]) {
    camera_move(camera, quaternion_up(camera->transform.rot), MOVEMENT_SPEED * delta_time);
  }
  if (pressed[42]) {
    camera_move(camera, quaternion_down(camera->transform.rot), MOVEMENT_SPEED * delta_time);
  }
  if (pressed[103]) {
    camera_rotate(camera, X_AXIS, SENSITIVITY * delta_time);
  }
  if (pressed[105]) {
    camera_rotate(camera, Y_AXIS, SENSITIVITY * delta_time);
  }
  if (pressed[108]) {
    camera_rotate(camera, X_AXIS, -SENSITIVITY * delta_time);
  }
  if (pressed[106]) {
    camera_rotate(camera, Y_AXIS, -SENSITIVITY * delta_time);
  }
  return 0;
}
