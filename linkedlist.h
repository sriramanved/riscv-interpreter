/**
 * Type alias for the internal representation of linkedlist.
 * Defined in linkedlist.c:
 *
 *     struct linkedlist {
 *         ...
 *     }
 *
 * And later reference this struct type using linkedlist_t.
 */
typedef struct linkedlist linkedlist_t;

/**
 * Return a pointer to a new linkedlist
 */
linkedlist_t *ll_init();

/**
 * Free a pre-existing linkedlist from memory
 */
void ll_free(linkedlist_t *list);

/**
 * Add a mapping from key->value to the associative linkedlist.
 * If a mapping with the given key already exists, replace the value.
 */
void ll_add(linkedlist_t *list, int key, int value);

/**
 * Retrieves the value associated with the given key from the linkedlist.
 * If the key does not exist in the linkedlist, return 0.
 */
int ll_get(linkedlist_t *list, int key);

/**
 * Returns the number of unique key->value mappings in the linkedlist.
 */
int ll_size(linkedlist_t *list);
