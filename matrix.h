#ifndef MATRIX_H_
#define MATRIX_H_
#include "vector.h"
#include "vertex.h"

typedef struct {
	float m[16];
} matrix;

matrix multiplyMatricies(matrix m1, matrix m2);

matrix initIdentifyMatrix();

matrix initScreenSpaceTransform(float halfWidth, float halfHeight);

matrix initMatrixTranslation(float x, float y, float z);

matrix initMatrixRotationAngle(float x, float y, float z, float angle);

matrix initMatrixRotation(float x, float y, float z);

matrix initMatrixRotationForwardUpRight(vector forward, vector up, vector right);

matrix initMatrixRotationForwardUp(vector forward, vector up);

matrix initScale(float x, float y, float z);

matrix initPerspective(float fov, float aspectRatio, float near, float far);

matrix initOrthographic(float left, float right, float bottom, float top, float near, float far);

vector matrixTransform(matrix m, vector r);

vertex vertexTransform(vertex v, matrix transform);

#endif
