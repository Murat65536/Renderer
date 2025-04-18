#include "vector.h"
#include <math.h>

vector vectorAddVector(vector v1, vector v2) {
	return (vector) {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

vector vectorAddFloat(vector v, float f) {
	return (vector) {v.x + f, v.y + f, v.z + f, v.w + f};
}

vector vectorSubtractVector(vector v1, vector v2) {
	return (vector) {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

vector vectorSubtractFloat(vector v, float f) {
	return (vector) {v.x - f, v.y - f, v.z - f, v.w - f};
}

vector vectorMultiplyVector(vector v1, vector v2) {
	return (vector) {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w};
}

vector vectorMultiplyFloat(vector v, float f) {
	return (vector) {v.x * f, v.y * f, v.z * f, v.w * f};
}

vector vectorDivideVector(vector v1, vector v2) {
	return (vector) {v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w};
}

vector vectorDivideFloat(vector v, float f) {
	return (vector) {v.x / f, v.y / f, v.z / f, v.w / f};
}

vector vectorAbs(vector v) {
	return (vector) {fabsf(v.x), fabsf(v.y), fabsf(v.z), fabsf(v.w)};
}

float vectorLength(vector v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

float vectorMax(vector v) {
	return fmaxf(fmaxf(v.x, v.y), fmaxf(v.z, v.w));
}

float vectorDotProduct(vector v1, vector v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

vector vectorCrossProduct(vector v1, vector v2) {
	float x = v1.y * v2.z - v1.z * v2.y;
	float y = v1.z * v2.x - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;

	return (vector) {x, y, z, 0};
}

vector vectorNormalized(vector v) {
	float length = vectorLength(v);
	return (vector) {v.x / length, v.y / length, v.z / length, v.w / length};
}

vector vectorRotate(vector v, vector axis, float angle) {
	float sine = sinf(-angle);
	float cosine = cosf(-angle);

	return vectorAddVector(vectorCrossProduct(v, vectorMultiplyFloat(axis, sine)), vectorAddVector(vectorMultiplyFloat(v, cosine), vectorMultiplyFloat(axis, vectorDotProduct(v, vectorMultiplyFloat(axis, 1 - cosine)))));
}

vector vectorLerp(vector v, vector dest, float lerpFactor) {
	return vectorAddVector(vectorMultiplyFloat(vectorSubtractVector(dest, v), lerpFactor), v);
}
