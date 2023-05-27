#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable
{
    int size;
    int capacity;
    linkedlist_t **buckets;
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
static int hash(int key, int max_range)
{
    key = (key > 0) ? key : -key;
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets)
{
    hashtable_t *ht = malloc(sizeof(hashtable_t));
    ht->size = 0;
    ht->capacity = num_buckets;
    ht->buckets = malloc(sizeof(linkedlist_t *) * num_buckets);

    for (int i = 0; i < num_buckets; i++)
    {
        ht->buckets[i] = ll_init();
    }
    return ht;
}

void ht_free(hashtable_t *table)
{
    for (int i = 0; i < table->capacity; i++)
    {
        ll_free(table->buckets[i]);
    }

    free(table->buckets);
    free(table);
}

void ht_add(hashtable_t *table, int key, int value)
{
    int index = hash(key, table->capacity);
    ll_add(table->buckets[index], key, value);
}

int ht_get(hashtable_t *table, int key)
{
    int index = hash(key, table->capacity);
    return ll_get(table->buckets[index], key);
}

int ht_size(hashtable_t *table)
{
    int temp = 0;
    for (int i = 0; i < table->capacity; i++) 
    {
        temp += ll_size(table->buckets[i]);
    }
    table->size = temp;
    return table->size;
}
