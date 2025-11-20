#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef b8 (*linked_list_predicate_t)(void *data, void *item);

typedef struct __linked_list_node linked_list_node_t;
struct __linked_list_node {
  void *data;

  linked_list_node_t *next;
  linked_list_node_t *previous;
};

typedef struct __linked_list {
  u64 count;
  u64 stride;

  linked_list_node_t *head;
  linked_list_node_t *tail;
} linked_list_t;

linked_list_t *linked_list_create(u64 stride);
b8 linked_list_destroy(linked_list_t *list);

linked_list_node_t *linked_list_node_create(u64 stride);
b8 linked_list_node_destroy(linked_list_node_t *node);

b8 linked_list_clear(linked_list_t *list);
b8 linked_list_insert(linked_list_t *list, u64 index, void *item);
b8 linked_list_remove(linked_list_t *list, u64 index, void *item);
b8 linked_list_retrieve(linked_list_t *list, u64 index, void *item);

b8 linked_list_find(linked_list_t *list, linked_list_predicate_t predicate, void *data, u64 *index, void *item);
