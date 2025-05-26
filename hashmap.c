#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

#define INITIAL_SIZE 16

hashmap_t *create_hashmap(size_t key_size, size_t value_size, bool (*compare)(void *a, void *b)) {
	hashmap_t *hashmap;
	hashmap = malloc(sizeof(*hashmap));
	hashmap->keys = malloc(sizeof(void*) * INITIAL_SIZE);
	hashmap->values = malloc(sizeof(void*) * INITIAL_SIZE);
	hashmap->compare = compare;
	hashmap->length = 0;
	hashmap->capacity = INITIAL_SIZE;
	hashmap->key_size = key_size;
	hashmap->value_size = value_size;
	return hashmap;
}

void *hashmap_get(hashmap_t *hashmap, void *key) {
	for (size_t i = 0; i < hashmap->length; i++) {
		if (hashmap->compare(key, hashmap->keys[i])) {
			return hashmap->values[i];
		}
	}
	return NULL;
}

void hashmap_set(hashmap_t *hashmap, void *key, void *value) {
	for (size_t i = 0; i < hashmap->length; i++) {
		if (hashmap->compare(key, hashmap->keys[i])) {
			hashmap->values[i] = value;
		}
	}
}

void hashmap_add(hashmap_t *hashmap, void *key, void *value) {
	if (hashmap->length >= hashmap->capacity) {
		hashmap->capacity *= 2;
		hashmap->keys = realloc(hashmap->keys, hashmap->capacity * sizeof(void*));
		hashmap->values = realloc(hashmap->values, hashmap->capacity * sizeof(void*));
	}
	hashmap->keys[hashmap->length] = malloc(hashmap->key_size);
	hashmap->values[hashmap->length] = malloc(hashmap->value_size);
	memcpy(hashmap->keys[hashmap->length], key, hashmap->key_size);
	memcpy(hashmap->values[hashmap->length], value, hashmap->value_size);
	//hashmap->keys[hashmap->length] = key;
	//hashmap->values[hashmap->length] = value;
	hashmap->length++;
}
