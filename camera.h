#ifndef CAMERA_H_
#define CAMERA_H_
#include "transform.h"
#include "matrix.h"
#include "vector.h"

typedef struct {
	transform_t transform;
	matrix_t projection;
} camera_t;

camera_t *create_camera();

matrix_t get_view_projection(camera_t *camera);

void camera_move(camera_t *camera, vector_t dir, float amount);

void camera_rotate(camera_t *camera, vector_t axis, float angle);

int camera_update(camera_t *camera, const float delta_time);

#endif
