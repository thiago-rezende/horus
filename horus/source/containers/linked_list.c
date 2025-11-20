/* horus containers layer */
#include <horus/containers/linked_list.h>

/* horus platform layer */
#include <horus/platform/memory.h>

linked_list_t *linked_list_create(u64 stride) {
  linked_list_t *list = (linked_list_t *)platform_memory_allocate(sizeof(linked_list_t));

  if (list == NULL) {
    return NULL;
  }

  *list = (linked_list_t){
      .count = 0,
      .stride = stride,
      .head = NULL,
      .tail = NULL,
  };

  return list;
}

b8 linked_list_destroy(linked_list_t *list) {
  if (list == NULL) {
    return false;
  }

  linked_list_clear(list);

  platform_memory_deallocate(list);

  return true;
}

linked_list_node_t *linked_list_node_create(u64 stride) {
  linked_list_node_t *node = (linked_list_node_t *)platform_memory_allocate(sizeof(linked_list_node_t));

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

b8 linked_list_node_destroy(linked_list_node_t *node) {
  if (node == NULL) {
    return false;
  }

  if (node->data != NULL) {
    platform_memory_deallocate(node->data);
  }

  platform_memory_deallocate(node);

  return true;
}

b8 linked_list_clear(linked_list_t *list) {
  if (list == NULL) {
    return false;
  }

  linked_list_node_t *current = list->head;

  while (current != NULL) {
    linked_list_node_t *next = current->next;

    linked_list_node_destroy(current);

    current = next;
  }

  list->head = NULL;
  list->tail = NULL;
  list->count = 0;

  return true;
}

b8 linked_list_insert(linked_list_t *list, u64 index, void *item) {
  if (list == NULL || item == NULL || index > list->count) {
    return false;
  }

  linked_list_node_t *node = linked_list_node_create(list->stride);

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

  linked_list_node_t *current = list->head;

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

b8 linked_list_remove(linked_list_t *list, u64 index, void *item) {
  if (list == NULL || list->count == 0 || index >= list->count) {
    return false;
  }

  linked_list_node_t *current = list->head;

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

  linked_list_node_destroy(current);

  list->count--;

  return true;
}

b8 linked_list_retrieve(linked_list_t *list, u64 index, void *item) {
  if (list == NULL || item == NULL) {
    return false;
  }

  if (index >= list->count) {
    return false;
  }

  linked_list_node_t *current = list->head;

  for (u64 i = 0; i < index; i++) {
    current = current->next;
  }

  if (!platform_memory_copy(item, current->data, list->stride)) {
    return false;
  }

  return true;
}

b8 linked_list_find(linked_list_t *list, linked_list_predicate_t predicate, void *data, u64 *index, void *item) {
  if (list == NULL || predicate == NULL) {
    return false;
  }

  if (list->count == 0) {
    return false;
  }

  u64 current_index = 0;
  linked_list_node_t *current = list->head;

  while (current != NULL) {
    if (predicate(data, current->data)) {
      if (index != NULL) {
        platform_memory_copy(index, &current_index, sizeof(u64));
      }

      if (item != NULL) {
        platform_memory_copy(item, current->data, list->stride);
      }

      return true;
    }

    current = current->next;

    current_index++;
  }

  return false;
}
