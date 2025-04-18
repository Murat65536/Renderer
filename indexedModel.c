#include "indexedModel.h"
#include "vector.h"

indexedModel newIndexedModel() {
	return (indexedModel) {
		newList(),
		newList(),
		newList(),
		newList(),
		newList()
	};
}

void calcNormals(indexedModel *l) {
	for (unsigned int i = 0; i < l->indices.length; i += 3) {
		int i0 = *(int *)l->indices.array[i];
		int i1 = *(int *)l->indices.array[i + 1];
		int i2 = *(int *)l->indices.array[i + 2];
		
		vector v1 = vectorSubtractVector(*(vector *)l->positions.array[i1], *(vector *)l->positions.array[i0]);
		vector v2 = vectorSubtractVector(*(vector *)l->positions.array[i2], *(vector *)l->positions.array[i0]);

		vector normal = vectorNormalized(vectorCrossProduct(v1, v2));

		vector normal1 = vectorAddVector(*(vector *)l->normals.array[i0], normal);
		vector normal2 = vectorAddVector(*(vector *)l->normals.array[i1], normal);
		vector normal3 = vectorAddVector(*(vector *)l->normals.array[i2], normal);
		l->normals.array[i0] = &normal1;
		l->normals.array[i1] = &normal2;
		l->normals.array[i2] = &normal3;
	}
	for (unsigned int i = 0; i < l->indices.length; i++) {
		vector vn = vectorNormalized(*(vector *)l->normals.array[i]);
		l->normals.array[i] = &vn;
	}
}

void calcTangents(indexedModel *l) {
	for (unsigned int i = 0; i < l->indices.length; i += 3) {
		int i0 = *(int *)l->indices.array[i];
		int i1 = *(int *)l->indices.array[i + 1];
		int i2 = *(int *)l->indices.array[i + 2];

		vector edge1 = vectorSubtractVector(*(vector *)l->positions.array[i1], *(vector *)l->positions.array[i0]);
		vector edge2 = vectorSubtractVector(*(vector *)l->positions.array[i2], *(vector *)l->positions.array[i0]);

		float deltaU1 = (*(vector *)l->texCoords.array[i1]).x - (*(vector *)l->texCoords.array[i0]).x;
		float deltaV1 = (*(vector *)l->texCoords.array[i1]).y - (*(vector *)l->texCoords.array[i0]).y;
		float deltaU2 = (*(vector *)l->texCoords.array[i2]).x - (*(vector *)l->texCoords.array[i0]).x;
		float deltaV2 = (*(vector *)l->texCoords.array[i2]).y - (*(vector *)l->texCoords.array[i0]).y;

		float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
		float f;
		if (dividend == 0.f) {
			f = 0.f;
		}
		else {
			f = 1.f / dividend;
		}

		vector tangent = {
			f * (deltaV2 * edge1.x - deltaV1 * edge2.x),
			f * (deltaV2 * edge1.y - deltaV1 * edge2.y),
			f * (deltaV2 * edge1.z - deltaV1 * edge2.z),
			0
		};

		vector tangent1 = vectorAddVector(*(vector *)l->tangents.array[i0], tangent);
		vector tangent2 = vectorAddVector(*(vector *)l->tangents.array[i1], tangent);
		vector tangent3 = vectorAddVector(*(vector *)l->tangents.array[i2], tangent);
		l->tangents.array[i0] = &tangent1;
		l->tangents.array[i1] = &tangent2;
		l->tangents.array[i2] = &tangent3;
	}
	for (unsigned int i = 0; i < l->tangents.length; i++) {
		vector vn = vectorNormalized(*(vector *)l->tangents.array[i]);
		l->tangents.array[i] = &vn;
	}
}
