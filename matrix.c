#include "matrix.h"
#include <math.h>

matrix multiplyMatricies(matrix m1, matrix m2) {
	return (matrix) {{
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

matrix initIdentifyMatrix() {
	return (matrix) {{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	}};
}


matrix initScreenSpaceTransform(float halfWidth, float halfHeight) {
	return (matrix) {{
		halfWidth, 0, 0, halfWidth,
		0, -halfHeight, 0, halfHeight,
		0, 0, 1, 0,
		0, 0, 0, 1
	}};
}

matrix initMatrixTranslation(float x, float y, float z) {
	return (matrix) {{
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	}};
}

matrix initMatrixRotationAngle(float x, float y, float z, float angle) {
	float sine = sinf(angle);
	float cosine = cosf(angle);

	return (matrix) {{
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

matrix initMatrixRotation(float x, float y, float z) {
	matrix rx = {{
		1, 0, 0, 0,
		0, cosf(x), -sinf(x), 0,
		0, sinf(x), cosf(x), 0,
		0, 0, 0, 1
	}};
	matrix ry = {{
		cosf(y), 0, -sinf(y), 0,
		0, 1, 0, 0,
		sinf(y), 0, cosf(y), 0,
		0, 0, 0, 1
	}};
	matrix rz = {{
		cosf(z), -sinf(z), 0, 0,
		sinf(z), cosf(z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1	
	}};

	return multiplyMatricies(rz, multiplyMatricies(ry, rx));
}

matrix initMatrixRotationForwardUpRight(vector forward, vector up, vector right) {
	return (matrix) {{
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		forward.x, forward.y, forward.z, 0,
		0, 0, 0, 1
	}};
}

matrix initMatrixRotationForwardUp(vector forward, vector up) {
	vector f = vectorNormalized(forward);
	vector r = vectorCrossProduct(vectorNormalized(up), f);
	vector u = vectorCrossProduct(f, r);
	return initMatrixRotationForwardUpRight(f, u, r);
}

matrix initScale(float x, float y, float z) {
	return (matrix) {{
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	}};
}

matrix initPerspective(float fov, float aspectRatio, float near, float far) {
	float tanHalfFov = tanf(fov / 2);
	float range = near - far;

	return (matrix) {{
		1.f / (tanHalfFov * aspectRatio), 0, 0, 0,
		0, 1.f / tanHalfFov, 0, 0,
		0, 0, (-near - far) / range, 2 * far * near / range,
		0, 0, 1, 0
	}};
}

matrix initOrthographic(float left, float right, float bottom, float top, float near, float far) {
	float width = right - left;
	float height = top - bottom;
	float depth = far - near;

	return (matrix) {{
		2.f / width, 0, 0, -(right + left) / width,
		0, 2.f / height, 0, -(top + bottom) / height,
		0, 0, -2.f / depth, -(far + near) / depth,
		0, 0, 0, 1
	}};
}

vector matrixTransform(matrix m, vector r) {
	return (vector) {
		m.m[0] * r.x + m.m[1] * r.y + m.m[2] * r.z + m.m[3] * r.w,
		m.m[4] * r.x + m.m[5] * r.y + m.m[6] * r.z + m.m[7] * r.w,
		m.m[8] * r.x + m.m[9] * r.y + m.m[10] * r.z + m.m[11] * r.w,
		m.m[12] * r.x + m.m[13] * r.y + m.m[14] * r.z + m.m[15] * r.w
	};
}

vertex vertexTransform(vertex v, matrix transform) {
	return (vertex) {matrixTransform(transform, v.pos), v.texCoords};
}
