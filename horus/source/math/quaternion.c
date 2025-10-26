/* horus math layer */
#include <horus/math/quaternion.h>

quaternionf32_t quaternionf32_add(quaternionf32_t a, quaternionf32_t b) {
  quaternionf32_t result;

  result.vector = a.vector + b.vector;

  return result;
}
