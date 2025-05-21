#include "indexed_model.h"
#include "vector.h"

indexed_model_t newIndexedModel() {
	return (indexed_model_t) {
		newList(),
		newList(),
		newList(),
		newList(),
		newList()
	};
}

void calcNormals(indexed_model_t *l) {
	for (unsigned int i = 0; i < l->indices.length; i += 3) {
		int i0 = *(int *)l->indices.array[i];
		int i1 = *(int *)l->indices.array[i + 1];
		int i2 = *(int *)l->indices.array[i + 2];
		
		vector_t v1 = vector_subtract_vector(*(vector_t *)l->positions.array[i1], *(vector_t *)l->positions.array[i0]);
		vector_t v2 = vector_subtract_vector(*(vector_t *)l->positions.array[i2], *(vector_t *)l->positions.array[i0]);

		vector_t normal = vector_normalized(vector_cross_product(v1, v2));

		l->normals.array[i0] = malloc_vector(vector_add_vector(*(vector_t *)l->normals.array[i0], normal));
		l->normals.array[i1] = malloc_vector(vector_add_vector(*(vector_t *)l->normals.array[ i1], normal));
		l->normals.array[i2] = malloc_vector(vector_add_vector(*(vector_t *)l->normals.array[i2], normal));
	}
	for (unsigned int i = 0; i < l->indices.length; i++) {
		l->normals.array[i] = malloc_vector(vector_normalized(*(vector_t *)l->normals.array[i]));
	}
}

void calcTangents(indexed_model_t *l) {
	for (unsigned int i = 0; i < l->indices.length; i += 3) {
		int i0 = *(int *)l->indices.array[i];
		int i1 = *(int *)l->indices.array[i + 1];
		int i2 = *(int *)l->indices.array[i + 2];

		vector_t edge1 = vector_subtract_vector(*(vector_t *)l->positions.array[i1], *(vector_t *)l->positions.array[i0]);
		vector_t edge2 = vector_subtract_vector(*(vector_t *)l->positions.array[i2], *(vector_t *)l->positions.array[i0]);

		float deltaU1 = (*(vector_t *)l->texCoords.array[i1]).x - (*(vector_t *)l->texCoords.array[i0]).x;
		float deltaV1 = (*(vector_t *)l->texCoords.array[i1]).y - (*(vector_t *)l->texCoords.array[i0]).y;
		float deltaU2 = (*(vector_t *)l->texCoords.array[i2]).x - (*(vector_t *)l->texCoords.array[i0]).x;
		float deltaV2 = (*(vector_t *)l->texCoords.array[i2]).y - (*(vector_t *)l->texCoords.array[i0]).y;

		float dividend = deltaU1 * deltaV2 - deltaU2 * deltaV1;
		float f;
		if (dividend == 0.f) {
			f = 0.f;
		}
		else {
			f = 1.f / dividend;
		}

		vector_t tangent = {
			f * (deltaV2 * edge1.x - deltaV1 * edge2.x),
			f * (deltaV2 * edge1.y - deltaV1 * edge2.y),
			f * (deltaV2 * edge1.z - deltaV1 * edge2.z),
			0
		};

		l->tangents.array[i0] = malloc_vector(vector_add_vector(*(vector_t *)l->tangents.array[i0], tangent));
		l->tangents.array[i1] = malloc_vector(vector_add_vector(*(vector_t *)l->tangents.array[i1], tangent));
		l->tangents.array[i2] = malloc_vector(vector_add_vector(*(vector_t *)l->tangents.array[i2], tangent));
	}
	for (unsigned int i = 0; i < l->tangents.length; i++) {
		l->tangents.array[i] = malloc_vector(vector_normalized(*(vector_t *)l->tangents.array[i]));
	}
}
