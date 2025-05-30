#include "matrix.h"
#include <math.h>

matrix_t multiply_matrices(matrix_t m1, matrix_t m2) {
	return (matrix_t) {{
		m1.m[0] * m2.m[0] + m1.m[1] * m2.m[4] + m1.m[2] * m2.m[8] + m1.m[3] * m2.m[12],
		m1.m[0] * m2.m[1] + m1.m[1] * m2.m[5] + m1.m[2] * m2.m[9] + m1.m[3] * m2.m[13],
		m1.m[0] * m2.m[2] + m1.m[1] * m2.m[6] + m1.m[2] * m2.m[10] + m1.m[3] * m2.m[14],
		m1.m[0] * m2.m[3] + m1.m[1] * m2.m[7] + m1.m[2] * m2.m[11] + m1.m[3] * m2.m[15],
		m1.m[4] * m2.m[0] + m1.m[5] * m2.m[4] + m1.m[6] * m2.m[8] + m1.m[7] * m2.m[12],
		m1.m[4] * m2.m[1] + m1.m[5] * m2.m[5] + m1.m[6] * m2.m[9] + m1.m[7] * m2.m[13],
		m1.m[4] * m2.m[2] + m1.m[5] * m2.m[6] + m1.m[6] * m2.m[10] + m1.m[7] * m2.m[14],
		m1.m[4] * m2.m[3] + m1.m[5] * m2.m[7] + m1.m[6] * m2.m[11] + m1.m[7] * m2.m[15],
		m1.m[8] * m2.m[0] + m1.m[9] * m2.m[4] + m1.m[10] * m2.m[8] + m1.m[11] * m2.m[12],
		m1.m[8] * m2.m[1] + m1.m[9] * m2.m[5] + m1.m[10] * m2.m[9] + m1.m[11] * m2.m[13],
		m1.m[8] * m2.m[2] + m1.m[9] * m2.m[6] + m1.m[10] * m2.m[10] + m1.m[11] * m2.m[14],
		m1.m[8] * m2.m[3] + m1.m[9] * m2.m[7] + m1.m[10] * m2.m[11] + m1.m[11] * m2.m[15],
		m1.m[12] * m2.m[0] + m1.m[13] * m2.m[4] + m1.m[14] * m2.m[8] + m1.m[15] * m2.m[12],
		m1.m[12] * m2.m[1] + m1.m[13] * m2.m[5] + m1.m[14] * m2.m[9] + m1.m[15] * m2.m[13],
		m1.m[12] * m2.m[2] + m1.m[13] * m2.m[6] + m1.m[14] * m2.m[10] + m1.m[15] * m2.m[14],
		m1.m[12] * m2.m[3] + m1.m[13] * m2.m[7] + m1.m[14] * m2.m[11] + m1.m[15] * m2.m[15]
	}};
}

matrix_t init_identify_matrix() {
	return (matrix_t) {{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	}};
}


matrix_t init_screen_space_transform(float half_width, float half_height) {
	return (matrix_t) {{
		half_width, 0.f, 0.f, half_width - 0.5f,
		0.f, -half_height, 0.f, half_height - 0.5f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	}};
}

matrix_t init_matrix_translation(float x, float y, float z) {
	return (matrix_t) {{
		1.f, 0.f, 0.f, x,
		0.f, 1.f, 0.f, y,
		0.f, 0.f, 1.f, z,
		0.f, 0.f, 0.f, 1.f
	}};
}

matrix_t init_matrix_rotation_angle(float x, float y, float z, float angle) {
	float sine = sinf(angle);
	float cosine = cosf(angle);

	return (matrix_t) {{
		cosine + x * x * (1.f - cosine),
		x * y * (1.f - cosine) - z * sine,
		x * z * (1.f - cosine) + y * sine,
		0.f,
		y * x * (1.f - cosine) + z * sine,
		cosine + y * y * (1.f - cosine),
		y * z * (1.f - cosine) - x * sine,
		0.f,
		z * x * (1.f - cosine) - y * sine,
		z * y * (1.f - cosine) + x * sine,
		cosine + z * z * (1.f - cosine),
		0.f,
		0.f,
		0.f,
		0.f,
		1.f
	}};
}

matrix_t init_matrix_rotation(float x, float y, float z) {
	matrix_t rx = {{
		1.f, 0.f, 0.f, 0.f,
		0.f, cosf(x), -sinf(x), 0.f,
		0.f, sinf(x), cosf(x), 0,
		0.f, 0.f, 0.f, 1.f
	}};
	matrix_t ry = {{
		cosf(y), 0.f, -sinf(y), 0.f,
		0.f, 1.f, 0.f, 0.f,
		sinf(y), 0.f, cosf(y), 0.f,
		0.f, 0.f, 0.f, 1.f
	}};
	matrix_t rz = {{
		cosf(z), -sinf(z), 0.f, 0.f,
		sinf(z), cosf(z), 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f	
	}};

	return multiply_matrices(rz, multiply_matrices(ry, rx));
}

matrix_t init_matrix_rotation_forward_up_right(vector_t forward, vector_t up, vector_t right) {
	return (matrix_t) {{
		right.x, right.y, right.z, 0.f,
		up.x, up.y, up.z, 0.f,
		forward.x, forward.y, forward.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	}};
}

matrix_t init_matrix_rotation_forward_up(vector_t forward, vector_t up) {
	vector_t f = vector_normalized(forward);
	vector_t r = vector_cross_product(vector_normalized(up), f);
	vector_t u = vector_cross_product(f, r);
	return init_matrix_rotation_forward_up_right(f, u, r);
}

matrix_t init_matrix_scale(float x, float y, float z) {
	return (matrix_t) {{
		x, 0.f, 0.f, 0.f,
		0.f, y, 0.f, 0.f,
		0.f, 0.f, z, 0.f,
		0.f, 0.f, 0.f, 1.f
	}};
}

matrix_t init_perspective(float fov, float aspect_ratio, float near, float far) {
	float tan_half_fov = tanf(fov / 2);
	float range = near - far;

	return (matrix_t) {{
		1.f / (tan_half_fov * aspect_ratio), 0.f, 0.f, 0.f,
		0.f, 1.f / tan_half_fov, 0.f, 0.f,
		0.f, 0.f, (-near - far) / range, 2.f * far * near / range,
		0.f, 0.f, 1.f, 0.f
	}};
}

matrix_t init_orthographic(float left, float right, float bottom, float top, float near, float far) {
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	return (matrix_t) {{
		2.f / width, 0.f, 0.f, -(right + left) / width,
		0.f, 2.f / height, 0.f, -(top + bottom) / height,
		0.f, 0.f, -2.f / depth, -(far + near) / depth,
		0.f, 0.f, 0.f, 1.f
	}};
}

vector_t matrix_transform(matrix_t m, vector_t r) {
	return (vector_t) {
		m.m[0] * r.x + m.m[1] * r.y + m.m[2] * r.z + m.m[3] * r.w,
		m.m[4] * r.x + m.m[5] * r.y + m.m[6] * r.z + m.m[7] * r.w,
		m.m[8] * r.x + m.m[9] * r.y + m.m[10] * r.z + m.m[11] * r.w,
		m.m[12] * r.x + m.m[13] * r.y + m.m[14] * r.z + m.m[15] * r.w
	};
}

vertex_t vertex_transform(vertex_t v, matrix_t transform) {
	return (vertex_t) {matrix_transform(transform, v.pos), v.tex_coords};
}
