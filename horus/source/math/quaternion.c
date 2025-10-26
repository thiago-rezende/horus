/* horus math layer */
#include <horus/math/quaternion.h>

quaternionf32_t quaternionf32_add(quaternionf32_t a, quaternionf32_t b) {
  quaternionf32_t result;

  result.vector = a.vector + b.vector;

  return result;
}

/* TODO: inline all possible quaternion functions */
quaternionf32_t quaternionf32_multiply(quaternionf32_t a, quaternionf32_t b) {
  quaternionf32_t result;

  /* TODO: improve with explicit vectorization */
  result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
  result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
  result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
  result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

  return result;
}
