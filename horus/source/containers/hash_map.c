/* horus core layer */
#include <horus/core/hash.h>
#include <horus/core/strings.h>

/* horus containers layer */
#include <horus/containers/hash_map.h>

/* horus platform layer */
#include <horus/platform/memory.h>

typedef struct __hash_map_clear_callback_state {
  void *state;
  container_callback_t callback;
} hash_map_clear_callback_state_t;

static b8 hash_map_node_predicate(void *data, void *item) {
  char *key = (char *)data;
  hash_map_node_t *node = (hash_map_node_t *)item;

  return string_compare_secure(key, node->key, HASH_MAP_MAX_KEY_LENGTH);
}

static b8 __hash_map_clear_callback(void *data, void *state);

hash_map_t *hash_map_create(u64 size, u64 stride) {
  hash_map_t *map = platform_memory_allocate(sizeof(hash_map_t));
  linked_list_t **lists = platform_memory_allocate(sizeof(linked_list_t *) * size);

  for (u64 i = 0; i < size; i++) {
    lists[i] = linked_list_create(sizeof(hash_map_node_t));
  }

  *map = (hash_map_t){
      .size = size,
      .count = 0,
      .stride = stride,

      .lists = lists,
  };

  return map;
}

b8 hash_map_destroy(hash_map_t *map) {
  if (map == NULL) {
    return false;
  }

  hash_map_clear_callback_state_t state = (hash_map_clear_callback_state_t){
      .state = NULL,
      .callback = NULL,
  };

  for (u64 i = 0; i < map->size; i++) {
    linked_list_t *list = map->lists[i];

    if (!linked_list_destroy_with_callback(list, __hash_map_clear_callback, (void *)&state)) {
      return false;
    }
  }

  platform_memory_deallocate(map->lists);
  platform_memory_deallocate(map);

  return true;
}

b8 hash_map_destroy_with_callback(hash_map_t *map, container_callback_t callback, void *state) {
  if (map == NULL) {
    return false;
  }

  hash_map_clear_callback_state_t clear_state = (hash_map_clear_callback_state_t){
      .state = state,
      .callback = callback,
  };

  for (u64 i = 0; i < map->size; i++) {
    linked_list_t *list = map->lists[i];

    if (!linked_list_destroy_with_callback(list, __hash_map_clear_callback, (void *)&clear_state)) {
      return false;
    }
  }

  platform_memory_deallocate(map->lists);
  platform_memory_deallocate(map);

  return true;
}

b8 hash_map_clear(hash_map_t *map) {
  if (map == NULL) {
    return false;
  }

  hash_map_clear_callback_state_t state = (hash_map_clear_callback_state_t){
      .state = NULL,
      .callback = NULL,
  };

  for (u64 i = 0; i < map->size; i++) {
    linked_list_t *list = map->lists[i];

    if (!linked_list_clear_with_callback(list, __hash_map_clear_callback, (void *)&state)) {
      return false;
    }
  }

  map->count = 0;

  return true;
}

b8 hash_map_insert(hash_map_t *map, const char *key, void *data) {
  if (map == NULL || key == NULL || data == NULL) {
    return false;
  }

  u64 key_length = string_length_secure((char *)key, HASH_MAP_MAX_KEY_LENGTH);

  u64 index = hash_fnv1a64(key, key_length) % map->size;

  linked_list_t *list = map->lists[index];

  if (linked_list_find(list, hash_map_node_predicate, (void *)key, NULL, NULL)) {
    /* TODO: implement value overwite */
    return true;
  }

  /* TODO: create functions to manage hash_map_node_t */
  hash_map_node_t node = (hash_map_node_t){
      .key = platform_memory_allocate(sizeof(u8) * key_length + 1),
      .data = platform_memory_allocate(sizeof(u8) * map->stride),
  };

  platform_memory_clear(node.key, sizeof(u8) * key_length + 1);

  platform_memory_copy(node.key, (void *)key, key_length);
  platform_memory_copy(node.data, data, sizeof(u8) * map->stride);

  if (!linked_list_insert(list, list->count, &node)) {
    platform_memory_deallocate(node.key);
    platform_memory_deallocate(node.data);

    return false;
  }

  map->count++;

  return true;
}

b8 hash_map_remove(hash_map_t *map, const char *key, void *data) {
  if (map == NULL || key == NULL) {
    return false;
  }

  u64 key_length = string_length_secure((char *)key, HASH_MAP_MAX_KEY_LENGTH);

  u64 index = hash_fnv1a64(key, key_length) % map->size;

  linked_list_t *list = map->lists[index];

  u64 item_index = 0;
  hash_map_node_t node_instance = {0};

  if (!linked_list_find(list, hash_map_node_predicate, (void *)key, &item_index, NULL)) {
    return false;
  }

  linked_list_remove(list, item_index, (void *)&node_instance);

  if (data != NULL) {
    platform_memory_copy(data, node_instance.data, map->stride);
  }

  /* TODO: create functions to manage hash_map_node_t */
  platform_memory_deallocate(node_instance.key);
  platform_memory_deallocate(node_instance.data);

  map->count--;

  return true;
}

b8 hash_map_retrieve(hash_map_t *map, const char *key, void *data) {
  if (map == NULL || key == NULL || data == NULL) {
    return false;
  }

  u64 key_length = string_length_secure((char *)key, HASH_MAP_MAX_KEY_LENGTH);

  u64 index = hash_fnv1a64(key, key_length) % map->size;

  linked_list_t *list = map->lists[index];

  u64 item_index = 0;
  hash_map_node_t node_instance = {0};

  if (!linked_list_find(list, hash_map_node_predicate, (void *)key, &item_index, (void *)&node_instance)) {
    return false;
  }

  platform_memory_copy(data, node_instance.data, map->stride);

  return true;
}

b8 hash_map_clear_with_callback(hash_map_t *map, container_callback_t callback, void *state) {
  if (map == NULL) {
    return false;
  }

  hash_map_clear_callback_state_t clear_state = (hash_map_clear_callback_state_t){
      .state = state,
      .callback = callback,
  };

  for (u64 i = 0; i < map->size; i++) {
    linked_list_t *list = map->lists[i];

    if (!linked_list_clear_with_callback(list, __hash_map_clear_callback, (void *)&clear_state)) {
      return false;
    }
  }

  map->count = 0;

  return true;
}

b8 __hash_map_clear_callback(void *data, void *state) {
  hash_map_node_t *node = (hash_map_node_t *)data;

  hash_map_clear_callback_state_t *clear_state = (hash_map_clear_callback_state_t *)state;

  if (clear_state->callback != NULL && !clear_state->callback(node->data, clear_state->state)) {
    return false;
  }

  platform_memory_deallocate(node->key);
  platform_memory_deallocate(node->data);

  return true;
}
