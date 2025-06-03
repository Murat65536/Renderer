#include "math.h"
#include "quaternion.h"
#include "vector.h"
#include "matrix.h"

quaternion_t quaternion_axis_angle(const vector_t axis, const float angle) {
	const float sin_half_angle = _sin(angle * 0.5f);
	const float cos_half_angle = _cos(angle * 0.5f);

	return (quaternion_t) {axis.x * sin_half_angle, axis.y * sin_half_angle, axis.z * sin_half_angle, cos_half_angle};
}

quaternion_t quaternion_matrix(const matrix_t rotation) {
	quaternion_t quaternion = {0};
	const float trace = rotation.m[0] + rotation.m[4] + rotation.m[9];

	if (trace > 0) {
		const float s = 0.5f / _sqrt(trace + 1.f);
		quaternion.x = (rotation.m[5] - rotation.m[8]) * s;
		quaternion.y = (rotation.m[7] - rotation.m[2]) * s;
		quaternion.z = (rotation.m[1] - rotation.m[3]) * s;
		quaternion.w = 0.25f / s;
	}
	else if (rotation.m[0] > rotation.m[4] && rotation.m[0] > rotation.m[9]) {
		const float s = 2.f * _sqrt(1.f + rotation.m[0] - rotation.m[4] - rotation.m[9]);
		quaternion.x = 0.25f * s;
		quaternion.y = (rotation.m[3] + rotation.m[1]) / s;
		quaternion.z = (rotation.m[7] + rotation.m[2]) / s;
		quaternion.w = (rotation.m[5] + rotation.m[8]) / s;
	}
	else if (rotation.m[4] > rotation.m[9]) {
		const float s = 2.f * _sqrt(1.f + rotation.m[4] - rotation.m[0] - rotation.m[9]);
		quaternion.x = (rotation.m[3] + rotation.m[1]) / s;
		quaternion.y = 0.25f * s;
		quaternion.z = (rotation.m[8] + rotation.m[5]) / s;
		quaternion.w = (rotation.m[7] - rotation.m[2]) / s;
	}
	else {
		const float s = 2.f * _sqrt(1.f + rotation.m[9] - rotation.m[0] - rotation.m[4]);
		quaternion.x = (rotation.m[7] + rotation.m[2]) / s;
		quaternion.y = (rotation.m[5] + rotation.m[8]) / s;
		quaternion.z = 0.25f * s;
		quaternion.w = (rotation.m[1] - rotation.m[3]) / s;
	}

	return quaternion_normalize(quaternion);
}

float quaternion_length(const quaternion_t quaternion) {
	return _sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

quaternion_t quaternion_normalize(const quaternion_t quaternion) {
	const float length = quaternion_length(quaternion);
	return (quaternion_t) {quaternion.x / length, quaternion.y / length, quaternion.z / length, quaternion.w / length};
}

quaternion_t quaternion_conjugate(const quaternion_t quaternion) {
	return (quaternion_t) {-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w};
}

quaternion_t quaternion_multiply_float(const quaternion_t quaternion, const float f) {
	return (quaternion_t) {quaternion.x * f, quaternion.y * f, quaternion.z * f, quaternion.w * f};
}

quaternion_t quaternion_multiply_quaternion(const quaternion_t quaternion, const quaternion_t other) {
	return (quaternion_t) {
		quaternion.x * other.w + quaternion.w * other.x + quaternion.y * other.z - quaternion.z * other.y,
		quaternion.y * other.w + quaternion.w * other.y + quaternion.z * other.x - quaternion.x * other.z,
		quaternion.z * other.w + quaternion.w * other.z + quaternion.x * other.y - quaternion.y * other.x,
		quaternion.w * other.w - quaternion.x * other.x - quaternion.y * other.y - quaternion.z * other.z
	};
}

quaternion_t quaternion_multiply_vector(const quaternion_t quaternion, const vector_t v) {
	return (quaternion_t) {
		quaternion.w * v.x + quaternion.y * v.z - quaternion.z * v.y,
		quaternion.w * v.y + quaternion.z * v.x - quaternion.x * v.z,
		quaternion.w * v.z + quaternion.x * v.y - quaternion.y * v.x,
		-quaternion.x * v.x - quaternion.y * v.y - quaternion.z * v.z
	};
}

quaternion_t quaternion_subtract_quaternion(const quaternion_t quaternion, const quaternion_t other) {
	return (quaternion_t) {quaternion.x - other.x, quaternion.y - other.y, quaternion.z - other.z, quaternion.w - other.w};
}

quaternion_t quaternion_add_quaternion(const quaternion_t quaternion, const quaternion_t other) {
	return (quaternion_t) {quaternion.x + other.x, quaternion.y + other.y, quaternion.z + other.z, quaternion.w + other.w};
}

matrix_t quaternion_to_rotation_matrix(const quaternion_t quaternion) {
	const vector_t forward = {
		2.f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y),
		2.f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x),
		1.f - 2.f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y),
		1.f
	};
	const vector_t up = {
		2.f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z),
		1.f - 2.f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z),
		2.f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x),
		1.f
	};
	const vector_t right = {
		1.f - 2.f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z),
		2.f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z),
		2.f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y),
		1.f
	};

	return init_matrix_rotation_forward_up_right(forward, up, right);
}

float quaternion_dot_product(const quaternion_t quaternion, const quaternion_t other) {
	return quaternion.x * other.x + quaternion.y * other.y + quaternion.z * other.z + quaternion.w * other.w;
}

quaternion_t quaternion_normalized_lerp(const quaternion_t quaternion, quaternion_t dest, const float lerp_factor, const bool shortest) {
	if (shortest && quaternion_dot_product(quaternion, dest) < 0) {
		dest = (quaternion_t) {-dest.x, -dest.y, -dest.z, -dest.w};
	}

	return quaternion_normalize(quaternion_add_quaternion(quaternion_multiply_float(quaternion_subtract_quaternion(dest, quaternion), lerp_factor), quaternion));
}

quaternion_t quaternion_spherical_lerp(const quaternion_t quaternion, quaternion_t dest, const float lerp_factor, const bool shortest) {

	float cos = quaternion_dot_product(quaternion, dest);
	
	if (shortest && cos < 0) {
		cos = -cos;
		dest = (quaternion_t) {-dest.x, -dest.y, -dest.z, -dest.w};
	}

	if (_abs(cos) >= 1 - 1e3f) {
		return quaternion_normalized_lerp(quaternion, dest, lerp_factor, false);
	}

	const float sin = _sqrt(1.f - cos * cos);
	const float angle = _atan2(sin, cos);
	const float inv_sin = 1.f / sin;

	const float src_factor = _sin((1.f - lerp_factor) * angle) * inv_sin;
	const float dest_factor = _sin(lerp_factor * angle) * inv_sin;

	return quaternion_add_quaternion(quaternion_multiply_float(quaternion, src_factor), quaternion_multiply_float(dest, dest_factor));
}

vector_t quaternion_forward(const quaternion_t quaternion) {
	const vector_t v = (vector_t) {0, 0, 1, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_back(const quaternion_t quaternion) {
	const vector_t v = (vector_t) {0, 0, -1, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_up(const quaternion_t quaternion) {
	const vector_t v = (vector_t) {0, 1, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_down(const quaternion_t quaternion) {
	const vector_t v = (vector_t) {0, -1, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_right(const quaternion_t quaternion) {
	const vector_t v = (vector_t) {1, 0, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t quaternion_left(const quaternion_t quaternion) {
	const vector_t v= (vector_t) {-1, 0, 0, 1};
	return vector_rotate_quaternion(v, quaternion);
}

vector_t vector_rotate_quaternion(const vector_t vector, const quaternion_t rotation) {
	const quaternion_t conjugate = quaternion_conjugate(rotation);

	const quaternion_t w = quaternion_multiply_quaternion(quaternion_multiply_vector(rotation, vector), conjugate);

	return (vector_t) {w.x, w.y, w.z, 1.f};
}
