/* horus containers layer */
#include <horus/containers/list.h>

/* horus platform layer */
#include <horus/platform/memory.h>

list_t *list_create(u64 stride) {
  list_t *list = (list_t *)platform_memory_allocate(sizeof(list_t));

  if (list == NULL) {
    return NULL;
  }

  *list = (list_t){
      .count = 0,
      .stride = stride,
      .head = NULL,
      .tail = NULL,
  };

  return list;
}

b8 list_destroy(list_t *list) {
  if (list == NULL) {
    return false;
  }

  list_clear(list);

  platform_memory_deallocate(list);

  return true;
}

list_node_t *list_node_create(u64 stride) {
  list_node_t *node = (list_node_t *)platform_memory_allocate(sizeof(list_node_t));

  if (node == NULL) {
    return NULL;
  }

  node->data = platform_memory_allocate(sizeof(u8) * stride);

  if (node->data == NULL) {
    platform_memory_deallocate(node);

    return NULL;
  }

  node->next = NULL;
  node->previous = NULL;

  return node;
}

b8 list_node_destroy(list_node_t *node) {
  if (node == NULL) {
    return false;
  }

  if (node->data != NULL) {
    platform_memory_deallocate(node->data);
  }

  platform_memory_deallocate(node);

  return true;
}

b8 list_clear(list_t *list) {
  if (list == NULL) {
    return false;
  }

  list_node_t *current = list->head;

  while (current != NULL) {
    list_node_t *next = current->next;

    list_node_destroy(current);

    current = next;
  }

  list->head = NULL;
  list->tail = NULL;
  list->count = 0;

  return true;
}

b8 list_insert(list_t *list, u64 index, void *item) {
  if (list == NULL || item == NULL || index > list->count) {
    return false;
  }

  list_node_t *node = list_node_create(list->stride);

  if (node == NULL) {
    return false;
  }

  platform_memory_copy(node->data, item, list->stride);

  if (list->count == 0) {
    list->head = node;
    list->tail = node;

    list->count++;

    return true;
  }

  if (index == 0) {
    node->next = list->head;
    list->head->previous = node;
    list->head = node;

    list->count++;

    return true;
  }

  if (index == list->count) {
    node->previous = list->tail;
    list->tail->next = node;
    list->tail = node;

    list->count++;

    return true;
  }

  list_node_t *current = list->head;

  for (u64 i = 0; i < index; i++) {
    current = current->next;
  }

  node->previous = current->previous;
  node->next = current;

  if (current->previous != NULL) {
    current->previous->next = node;
  }

  current->previous = node;

  list->count++;

  return true;
}

b8 list_remove(list_t *list, u64 index, void *item) {
  if (list == NULL || list->count == 0 || index >= list->count) {
    return false;
  }

  list_node_t *current = list->head;

  for (u64 i = 0; i < index; i++) {
    current = current->next;
  }

  if (current->previous != NULL) {
    current->previous->next = current->next;
  } else {
    list->head = current->next;
  }

  if (current->next != NULL) {
    current->next->previous = current->previous;
  } else {
    list->tail = current->previous;
  }

  if (item != NULL) {
    platform_memory_copy(item, current->data, list->stride);
  }

  list_node_destroy(current);

  list->count--;

  return true;
}

b8 list_retrieve(list_t *list, u64 index, void *item) {
  if (list == NULL || item == NULL) {
    return false;
  }

  if (index >= list->count) {
    return false;
  }

  list_node_t *current = list->head;

  for (u64 i = 0; i < index; i++) {
    current = current->next;
  }

  if (!platform_memory_copy(item, current->data, list->stride)) {
    return false;
  }

  return true;
}
