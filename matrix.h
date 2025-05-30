#ifndef MATRIX_H_
#define MATRIX_H_
#include "vector.h"
#include "vertex.h"

typedef struct {
	float m[16];
} matrix_t;

matrix_t multiply_matrices(matrix_t m1, matrix_t m2);

matrix_t init_identify_matrix();

matrix_t init_screen_space_transform(float half_width, float half_height);

matrix_t init_matrix_translation(float x, float y, float z);

matrix_t init_matrix_rotation_angle(float x, float y, float z, float angle);

matrix_t init_matrix_rotation(float x, float y, float z);

matrix_t init_matrix_rotation_forward_up_right(vector_t forward, vector_t up, vector_t right);

matrix_t init_matrix_rotation_forward_up(vector_t forward, vector_t up);

matrix_t init_matrix_scale(float x, float y, float z);

matrix_t init_perspective(float fov, float aspect_ratio, float near, float far);

matrix_t init_orthographic(float left, float right, float bottom, float top, float near, float far);

vector_t matrix_transform(matrix_t m, vector_t r);

vertex_t vertex_transform(vertex_t v, matrix_t transform);

#endif
