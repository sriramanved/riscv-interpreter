/**
 * Type alias for the internal representation of hashtable.
 * Defined in hashtable.c:
 *
 *     struct hashtable {
 *         ...
 *     }
 *
 * And later reference this struct type using hashtable_t.
 */
typedef struct hashtable hashtable_t;

/**
 * Return a pointer to a new hashtable that has been initialized with a fixed
 * number of buckets
 */
hashtable_t *ht_init(int num_buckets);

/**
 * Free a pre-existing hashtable from memory
 */
void ht_free(hashtable_t *table);

/**
 * Add a mapping from key->value to the hashtable.
 */
void ht_add(hashtable_t *table, int key, int value);

/**
 * Retrieves the value associated with the given key from the hashtable.
 * If the key does not exist in the hashtable, return 0.
 */
int ht_get(hashtable_t *table, int key);

/**
 * Returns the number of unique key->value mappings in the hashtable.
 */
int ht_size(hashtable_t *table);
