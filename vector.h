#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vector_t;

vector_t *malloc_vector(vector_t v);

vector_t vector_add_vector(vector_t v1, vector_t v2);

vector_t vector_add_float(vector_t v, float f);

vector_t vector_subtract_vector(vector_t v1, vector_t v2);

vector_t vector_subtract_float(vector_t v, float f);

vector_t vector_multiply_vector(vector_t v1, vector_t v2);

vector_t vector_multiply_float(vector_t v, float f);

vector_t vector_divide_vector(vector_t v1, vector_t v2);

vector_t vector_divide_float(vector_t v, float f);

vector_t vector_abs(vector_t v);

float vector_length(vector_t v);

float vector_max(vector_t v);

float vector_dot_product(vector_t v1, vector_t v2);

vector_t vector_cross_product(vector_t v1, vector_t v2);

vector_t vector_normalized(vector_t v);

vector_t vector_rotate(vector_t v, vector_t axis, float angle);

vector_t vector_lerp(vector_t v, vector_t dest, float lerp_factor);

#endif
