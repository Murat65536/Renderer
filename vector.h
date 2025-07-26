#ifndef VECTOR_H_
#define VECTOR_H_
#include "math.h"

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vector_t;

static inline vector_t vector_add_vector(vector_t v1, vector_t v2) {
	return (vector_t) {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

static inline vector_t vector_add_float(vector_t v, float f) {
	return (vector_t) {v.x + f, v.y + f, v.z + f, v.w + f};
}

static inline vector_t vector_subtract_vector(vector_t v1, vector_t v2) {
	return (vector_t) {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

static inline vector_t vector_subtract_float(vector_t v, float f) {
	return (vector_t) {v.x - f, v.y - f, v.z - f, v.w - f};
}

static inline vector_t vector_multiply_vector(vector_t v1, vector_t v2) {
	return (vector_t) {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

static inline vector_t vector_multiply_float(vector_t v, float f) {
	return (vector_t) {v.x * f, v.y * f, v.z * f, v.w * f};
}

static inline vector_t vector_divide_vector(vector_t v1, vector_t v2) {
	return (vector_t) {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

static inline vector_t vector_divide_float(vector_t v, float f) {
	return (vector_t) {v.x / f, v.y / f, v.z / f, v.w / f};
}

static inline vector_t vector_abs(vector_t v) {
	return (vector_t) {_abs(v.x), _abs(v.y), _abs(v.z), _abs(v.w)};
}

static inline float vector_length(vector_t v) {
	return _sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

static inline float vector_max(vector_t v) {
	return _max(_max(v.x, v.y), _max(v.z, v.w));
}

static inline float vector_min(vector_t v) {
  return _min(_min(v.x, v.y), _min(v.z, v.w));
}

static inline float vector_dot_product(vector_t v1, vector_t v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

static inline vector_t vector_cross_product(vector_t v1, vector_t v2) {
	return (vector_t) {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x, 0};
}

static inline vector_t vector_normalize(vector_t v) {
	float length = vector_length(v);
	return (vector_t) {v.x / length, v.y / length, v.z / length, v.w / length};
}

static inline vector_t vector_rotate(vector_t v, vector_t axis, float angle) {
	float sine = _sin(-angle);
	float cosine = _cos(-angle);

	return vector_add_vector(vector_cross_product(v, vector_multiply_float(axis, sine)), vector_add_vector(vector_multiply_float(v, cosine), vector_multiply_float(axis, vector_dot_product(v, vector_multiply_float(axis, 1 - cosine)))));
}

static inline vector_t vector_lerp(vector_t v, vector_t dest, float lerp_factor) {
	return vector_add_vector(vector_multiply_float(vector_subtract_vector(dest, v), lerp_factor), v);
}

#endif
