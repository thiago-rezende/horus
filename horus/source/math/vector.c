/* horus math layer */
#include <horus/math/vector.h>

vector3f32_t vector3f32_add(vector3f32_t a, vector3f32_t b) {
  vector3f32_t result;

  result.vector = a.vector + b.vector;

  return result;
}

vector4f32_t vector4f32_add(vector4f32_t a, vector4f32_t b) {
  vector4f32_t result;

  result.vector = a.vector + b.vector;

  return result;
}
