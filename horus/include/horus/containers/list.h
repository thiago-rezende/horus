#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef struct __list_node list_node_t;

struct __list_node {
  void *data;

  list_node_t *next;
  list_node_t *previous;
};

typedef struct __list {
  u64 count;
  u64 stride;

  list_node_t *head;
  list_node_t *tail;
} list_t;

list_t *list_create(u64 stride);
b8 list_destroy(list_t *list);

list_node_t *list_node_create(u64 stride);
b8 list_node_destroy(list_node_t *node);

b8 list_clear(list_t *list);
b8 list_insert(list_t *list, u64 index, void *item);
b8 list_remove(list_t *list, u64 index, void *item);
b8 list_retrieve(list_t *list, u64 index, void *item);
