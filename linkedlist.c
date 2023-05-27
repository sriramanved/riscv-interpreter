#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    int size;
};

struct linkedlist_node
{
    int value;
    struct linkedlist_node *next;
    int key;
};

typedef struct linkedlist_node linkedlist_node_t;

linkedlist_t *ll_init()
{
    linkedlist_t *list = malloc(sizeof(linkedlist_t));
    list->first = NULL;
    list->size = 0;
    return list;
};

void ll_free(linkedlist_t *list)
{
    linkedlist_node_t *current = list->first;
    while (current != NULL)
    {
        linkedlist_node_t *next = current->next;
        free(current);
        current = next;
    }
    free(list);
};

void ll_add(linkedlist_t *list, int key, int value)
{
    linkedlist_node_t *current = list->first;
    while (current != NULL)
    {
        if (current->key == key)
        {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // prepend node if the key doesn't exist.
    linkedlist_node_t *new_node = malloc(sizeof(linkedlist_node_t));
    new_node->key = key;
    new_node->value = value;
    new_node->next = list->first;
    list->first = new_node;
    list->size++;
};

int ll_get(linkedlist_t *list, int key)
{
    linkedlist_node_t *current = list -> first;
    while (current != NULL)
    {
        if (current->key == key)
        {
            return current->value;
        }
        current = current->next;
    }
    return 0;
};

int ll_size(linkedlist_t *list)
{
    return list->size;
};
