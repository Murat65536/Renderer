#include <math.h>
#include "quaternion.h"
#include "vector.h"
#include "matrix.h"

quaternion_t quaternion_axis_angle(vector_t axis, float angle) {
	float sin_half_angle = sinf(angle * 0.5f);
	float cos_half_angle = cosf(angle * 0.5f);

	return (quaternion_t) {axis.x * sin_half_angle, axis.y * sin_half_angle, axis.z * sin_half_angle, cos_half_angle};
}

quaternion_t quaternion_matrix(matrix_t rotation) {
	quaternion_t quaternion = {0};
	float trace = rotation.m[0] + rotation.m[4] + rotation.m[9];

	if (trace > 0) {
		float s = 0.5f / fsqrt(trace + 1.f);
		quaternion.x = (rotation.m[5] - rotation.m[8]) * s;
		quaternion.y = (rotation.m[7] - rotation.m[2]) * s;
		quaternion.z = (rotation.m[1] - rotation.m[3]) * s;
		quaternion.w = 0.25f / s;
	}
	else if (rotation.m[0] > rotation.m[4] && rotation.m[0] > rotation.m[9]) {
		float s = 2.f * fsqrt(1.f + rotation.m[0] - rotation.m[4] - rotation.m[9]);
		quaternion.x = 0.25f * s;
		quaternion.y = (rotation.m[3] + rotation.m[1]) / s;
		quaternion.z = (rotation.m[7] + rotation.m[2]) / s;
		quaternion.w = (rotation.m[5] + rotation.m[8]) / s;
	}
	else if (rotation.m[4] > rotation.m[9]) {
		float s = 2.f * fsqrt(1.f + rotation.m[4] - rotation.m[0] - rotation.m[9]);
		quaternion.x = (rotation.m[3] + rotation.m[1]) / s;
		quaternion.y = 0.25f * s;
		quaternion.z = (rotation.m[8] + rotation.m[5]) / s;
		quaternion.w = (rotation.m[7] - rotation.m[2]) / s;
	}
	else {
		float s = 2.f * fsqrt(1.f + rotation.m[9] - rotation.m[0] - rotation.m[4]);
		quaternion.x = (rotation.m[7] + rotation.m[2]) / s;
		quaternion.y = (rotation.m[5] + rotation.m[8]) / s;
		quaternion.z = 0.25f * s;
		quaternion.w = (rotation.m[1] - rotation.m[3]) / s;
	}

	return quaternion_normalize(quaternion);
}

float quaternion_length(quaternion_t quaternion) {
	return fsqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

quaternion_t quaternion_normalize(quaternion_t quaternion) {
	float length = quaternion_length(quaternion);

	return (quaternion_t) {quaternion.x / length, quaternion.y / length, quaternion.z / length, quaternion.w / length};
}

quaternion_t quaternion_conjugate(quaternion_t quaternion) {
	return (quaternion_t) {-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w};
}

quaternion_t quaternion_multiply_float(quaternion_t quaternion, float f) {
	return (quaternion_t) {quaternion.x * f, quaternion.y * f, quaternion.z * f, quaternion.w * f};
}

quaternion_t quaternion_multiply_quaternion(quaternion_t quaternion, quaternion_t other) {
	return (quaternion_t) {
		quaternion.x * other.w + quaternion.w * other.x + quaternion.y * other.z - quaternion.z * other.y,
		quaternion.y * other.w + quaternion.w * other.y + quaternion.z * other.x - quaternion.x * other.z,
		quaternion.z * other.w + quaternion.w * other.z + quaternion.x * other.y - quaternion.y * other.x,
		quaternion.w * other.w - quaternion.x * other.x - quaternion.y * other.y - quaternion.z * other.z
	};
}

quaternion_t quaternion_multiply_vector(quaternion_t quaternion, vector_t v) {
	return (quaternion_t) {
		quaternion.w * v.x + quaternion.y * v.z - quaternion.z * v.y,
		quaternion.w * v.y + quaternion.z * v.x - quaternion.x * v.z,
		quaternion.w * v.z + quaternion.x * v.y - quaternion.y * v.x,
		-quaternion.x * v.x - quaternion.y * v.y - quaternion.z * v.z
	};
}

quaternion_t quaternion_subtract_quaternion(quaternion_t quaternion, quaternion_t other) {
	return (quaternion_t) {quaternion.x - other.x, quaternion.y - other.y, quaternion.z - other.z, quaternion.w - other.w};
}

quaternion_t quaternion_add_quaternion(quaternion_t quaternion, quaternion_t other) {
	return (quaternion_t) {quaternion.x + other.x, quaternion.y + other.y, quaternion.z + other.z, quaternion.w + other.w};
}

matrix_t quaternion_to_rotation_matrix(quaternion_t quaternion) {
	vector_t forward = {
		2.f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y),
		2.f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x),
		1.f - 2.f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y),
		1.f
	};
	vector_t up = {
		2.f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z),
		1.f - 2.f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z),
		2.f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x),
		1.f
	};
	vector_t right = {
		1.f - 2.f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z),
		2.f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z),
		2.f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y),
		1.f
	};

	return init_matrix_rotation_forward_up_right(forward, up, right);
}

float quaternion_dot_product(quaternion_t quaternion, quaternion_t other) {
	return quaternion.x * other.x + quaternion.y * other.y + quaternion.z * other.z + quaternion.w * other.w;
}

quaternion_t quaternion_normalized_lerp(quaternion_t quaternion, quaternion_t dest, float lerp_factor, bool shortest) {
	quaternion_t corrected_dest = dest;
	if (shortest && quaternion_dot_product(quaternion, dest) < 0) {
		corrected_dest = (quaternion_t) {-dest.x, -dest.y, -dest.z, -dest.w};
	}

	return quaternion_normalize(quaternion_add_quaternion(quaternion_multiply_float(quaternion_subtract_quaternion(corrected_dest, quaternion), lerp_factor), quaternion));
}

quaternion_t quaternion_spherical_lerp(quaternion_t quaternion, quaternion_t dest, float lerp_factor, bool shortest) {
	const float EPSILON = 1e3f;

	float cos = quaternion_dot_product(quaternion, dest);
	quaternion_t corrected_dest = dest;
	
	if (shortest && cos < 0) {
		cos = -cos;
		corrected_dest = (quaternion_t) {-dest.x, -dest.y, -dest.z, -dest.w};
	}

	if (fabsf(cos) >= 1 - EPSILON) {
		return quaternion_normalized_lerp(quaternion, corrected_dest, lerp_factor, false);
	}

	float sin = fsqrt(1.f - cos * cos);
	float angle = atan2(sin, cos);
	float inv_sin = 1.f / sin;

	float src_factor = sinf((1.f - lerp_factor) * angle) * inv_sin;
	float dest_factor = sinf(lerp_factor * angle) * inv_sin;

	return quaternion_add_quaternion(quaternion_multiply_float(quaternion, src_factor), quaternion_multiply_float(corrected_dest, dest_factor));
}

vector_t quaternion_forward(quaternion_t quaternion) {
	vector_t v = (vector_t) {0, 0, 1, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_back(quaternion_t quaternion) {
	vector_t v = (vector_t) {0, 0, -1, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_up(quaternion_t quaternion) {
	vector_t v = (vector_t) {0, 1, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_down(quaternion_t quaternion) {
	vector_t v = (vector_t) {0, -1, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_right(quaternion_t quaternion) {
	vector_t v = (vector_t) {1, 0, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_left(quaternion_t quaternion) {
	vector_t v= (vector_t) {-1, 0, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t vector_rotate_quaternion(vector_t vector, quaternion_t rotation) {
	quaternion_t conjugate = quaternion_conjugate(rotation);

	quaternion_t w = quaternion_multiply_quaternion(quaternion_multiply_vector(rotation, vector), conjugate);

	return (vector_t) {w.x, w.y, w.z, 1.f};
}
