#include <stdio.h>
#include "linkedlist.h"

int main() {
    linkedlist_t *list = ll_init();
    printf("Adding mapping from 10 -> 123\n");
    ll_add(list, 10, 123);
    printf("Get 10 -> %d (expected 123)\n", ll_get(list, 10));
    ll_add(list, 10, 256);
    printf("Adding mapping from 10 -> 256\n");
    printf("Get 10 -> %d (expected 256)\n", ll_get(list, 10));
    printf("Size = %d (expected 1)\n", ll_size(list));
    printf("Adding mapping from 20 -> 9\n");
    ll_add(list, 20, 9);
    printf("Get 20 -> %d (expected 9)\n", ll_get(list, 20));
    printf("Size = %d (expected 2)\n", ll_size(list));
    ll_free(list);
}
