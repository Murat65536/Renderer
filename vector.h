#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vector;

vector vectorAddVector(vector v1, vector v2);

vector vectorAddFloat(vector v, float f);

vector vectorSubtractVector(vector v1, vector v2);

vector vectorSubtractFloat(vector v, float f);

vector vectorMultiplyVector(vector v1, vector v2);

vector vectorMultiplyFloat(vector v, float f);

vector vectorDivideVector(vector v1, vector v2);

vector vectorDivideFloat(vector v, float f);

vector vectorAbs(vector v);

float vectorLength(vector v);

float vectorMax(vector v);

float vectorDotProduct(vector v1, vector v2);

vector vectorCrossProduct(vector v1, vector v2);

vector vectorNormalized(vector v);

vector vectorRotate(vector v, vector axis, float angle);

vector vectorLerp(vector v, vector dest, float lerpFactor);

#endif
