/* horus renderer layer */
#include <horus/renderer/material.h>

const char *material_type_strings[MATERIAL_TYPE_COUNT] = {
    [MATERIAL_TYPE_DEFAULT] = "default",
    [MATERIAL_TYPE_UNLIT] = "unlit",
};

material_t *material_create(material_type_t type, renderer_t *renderer) {
  return NULL;
}

b8 material_destroy(material_t *material) {
  return false;
}

b8 material_bind(material_t *material, renderer_t *renderer) {
  return false;
}

const char *material_type_string(material_type_t type) {
  if (type < MATERIAL_TYPE_COUNT && type >= MATERIAL_TYPE_DEFAULT) {
    return material_type_strings[type];
  }

  return "unknown";
}
