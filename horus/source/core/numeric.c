/* horus core layer */
#include <horus/core/numeric.h>

u32 numeric_clamp_u32(u32 value, u32 minimum, u32 maximum) {
  if (value < minimum) {
    return minimum;
  }

  if (value > maximum) {
    return maximum;
  }

  return value;
}
