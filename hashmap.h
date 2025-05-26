typedef struct {
	void **keys;
	void **values;
	bool (*compare)(void *a, void *b);
	size_t length;
	size_t capacity;
	size_t key_size;
	size_t value_size;
} hashmap_t;

hashmap_t *create_hashmap(size_t key_size, size_t value_size, bool (*compare)(void *a, void *b));

void *hashmap_get(hashmap_t *hashmap, void *key);

void hashmap_set(hashmap_t *hashmap, void *key, void *value);

void hashmap_add(hashmap_t *hashmap, void *key, void *value);
