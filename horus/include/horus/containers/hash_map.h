#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus containers layer */
#include <horus/containers/linked_list.h>

#define HASH_MAP_MAX_KEY_LENGTH 255

typedef struct __hash_map_node {
  char *key;
  void *data;
} hash_map_node_t;

typedef struct __hash_map {
  u64 size;
  u64 count;
  u64 stride;

  linked_list_t **lists;
} hash_map_t;

hash_map_t *hash_map_create(u64 size, u64 stride);
b8 hash_map_destroy(hash_map_t *map);

b8 hash_map_insert(hash_map_t *map, const char *key, void *data);
b8 hash_map_remove(hash_map_t *map, const char *key, void *data);
b8 hash_map_retrieve(hash_map_t *map, const char *key, void *data);
