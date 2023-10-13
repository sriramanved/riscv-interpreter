#include <stdio.h>
#include "hashtable.h"

int main() {
    hashtable_t *table = ht_init(16);
    printf("Adding mapping from 10 -> 123\n");
    ht_add(table, 10, 123);
    printf("Get 10 -> %d (expected 123)\n", ht_get(table, 10));
    ht_add(table, 10, 256);
    printf("Adding mapping from 10 -> 256\n");
    printf("Get 10 -> %d (expected 256)\n", ht_get(table, 10));
    printf("Size = %d (expected 1)\n", ht_size(table));
    printf("Adding mapping from 20 -> 9\n");
    ht_add(table, 20, 9);
    printf("Get 20 -> %d (expected 9)\n", ht_get(table, 20));
    printf("Size = %d (expected 2)\n", ht_size(table));
    ht_free(table);
}
