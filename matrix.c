#include "matrix.h"
#include <math.h>

matrix_t multiply_matricies(matrix_t m1, matrix_t m2) {
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
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}};
}


matrix_t init_screen_space_transform(float half_width, float half_height) {
	return (matrix_t) {{
		half_width, 0, 0, half_width,
		0, -half_height, 0, half_height,
		0, 0, 1, 0,
		0, 0, 0, 1
	}};
}

matrix_t init_matrix_translation(float x, float y, float z) {
	return (matrix_t) {{
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	}};
}

matrix_t init_matrix_rotation_angle(float x, float y, float z, float angle) {
	float sine = sinf(angle);
	float cosine = cosf(angle);

	return (matrix_t) {{
		cosine + x * x * (1 - cosine),
		x * y * (1 - cosine) - z * sine,
		x * z * (1 - cosine) + y * sine,
		0,
		y * x * (1 - cosine) + z * sine,
		cosine + y * y * (1 - cosine),
		y * z * (1 - cosine) - x * sine,
		0,
		z * x * (1 - cosine) - y * sine,
		z * y * (1 - cosine) + x * sine,
		cosine + z * z * (1 - cosine),
		0,
		0,
		0,
		0,
		1
	}};
}

matrix_t init_matrix_rotation(float x, float y, float z) {
	matrix_t rx = {{
		1, 0, 0, 0,
		0, cosf(x), -sinf(x), 0,
		0, sinf(x), cosf(x), 0,
		0, 0, 0, 1
	}};
	matrix_t ry = {{
		cosf(y), 0, -sinf(y), 0,
		0, 1, 0, 0,
		sinf(y), 0, cosf(y), 0,
		0, 0, 0, 1
	}};
	matrix_t rz = {{
		cosf(z), -sinf(z), 0, 0,
		sinf(z), cosf(z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1	
	}};

	return multiply_matricies(rz, multiply_matricies(ry, rx));
}

matrix_t init_matrix_rotation_forward_up_right(vector_t forward, vector_t up, vector_t right) {
	return (matrix_t) {{
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		forward.x, forward.y, forward.z, 0,
		0, 0, 0, 1
	}};
}

matrix_t init_matrix_rotation_forward_up(vector_t forward, vector_t up) {
	vector_t f = vector_normalized(forward);
	vector_t r = vector_cross_product(vector_normalized(up), f);
	vector_t u = vector_cross_product(f, r);
	return init_matrix_rotation_forward_up_right(f, u, r);
}

matrix_t init_scale(float x, float y, float z) {
	return (matrix_t) {{
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	}};
}

matrix_t init_perspective(float fov, float aspect_ratio, float near, float far) {
	float tan_half_fov = tanf(fov / 2);
	float range = near - far;

	return (matrix_t) {{
		1.f / (tan_half_fov * aspect_ratio), 0, 0, 0,
		0, 1.f / tan_half_fov, 0, 0,
		0, 0, (-near - far) / range, 2 * far * near / range,
		0, 0, 1, 0
	}};
}

matrix_t init_orthographic(float left, float right, float bottom, float top, float near, float far) {
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	return (matrix_t) {{
		2.f / width, 0, 0, -(right + left) / width,
		0, 2.f / height, 0, -(top + bottom) / height,
		0, 0, -2.f / depth, -(far + near) / depth,
		0, 0, 0, 1
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
