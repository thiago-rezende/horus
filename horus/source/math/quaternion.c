#include <math.h>

/* horus math layer */
#include <horus/math/quaternion.h>

quaternionf32_t quaternionf32_add(quaternionf32_t a, quaternionf32_t b) {
  quaternionf32_t result;

  result.vector = a.vector + b.vector;

  return result;
}

quaternionf32_t quaternionf32_identity(void) {
  quaternionf32_t result = (quaternionf32_t){
      .x = 0,
      .y = 0,
      .z = 0,
      .w = 1,
  };

  return result;
}

quaternionf32_t quaternionf32_multiply(quaternionf32_t a, quaternionf32_t b) {
  quaternionf32_t result;

  /* TODO: improve with explicit vectorization */
  result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
  result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
  result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
  result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;

  return result;
}

quaternionf32_t quaternionf32_rotate_euler(quaternionf32_t quaternion, vector3f32_t degrees) {
  quaternionf32_t result;
  quaternionf32_t rotation;

  f32 radians_x = degrees.x * (pi_f32 / 180.0f);
  f32 radians_y = degrees.y * (pi_f32 / 180.0f);
  f32 radians_z = degrees.z * (pi_f32 / 180.0f);

  f32 half_x = radians_x * 0.5f;
  f32 half_y = radians_y * 0.5f;
  f32 half_z = radians_z * 0.5f;

  f32 sx = sinf(half_x);
  f32 cx = cosf(half_x);
  f32 sy = sinf(half_y);
  f32 cy = cosf(half_y);
  f32 sz = sinf(half_z);
  f32 cz = cosf(half_z);

  f32 cy_cz = cy * cz;
  f32 sy_sz = sy * sz;
  f32 cy_sz = cy * sz;
  f32 sy_cz = sy * cz;

  /* TODO: improve with explicit vectorization */
  rotation.x = sx * cy_cz + cx * sy_sz;
  rotation.y = cx * sy_cz - sx * cy_sz;
  rotation.z = cx * cy_sz + sx * sy_cz;
  rotation.w = cx * cy_cz - sx * sy_sz;

  result = quaternionf32_multiply(quaternion, rotation);

  return result;
}

matrix4f32_t quaternionf32_to_matrix(quaternionf32_t quaternion) {
  matrix4f32_t result = matrix4f32_identity();

  f32 x = quaternion.x;
  f32 y = quaternion.y;
  f32 z = quaternion.z;
  f32 w = quaternion.w;

  f32 x2 = x + x;
  f32 y2 = y + y;
  f32 z2 = z + z;

  f32 xx = x * x2;
  f32 yy = y * y2;
  f32 zz = z * z2;

  f32 xy = x * y2;
  f32 xz = x * z2;
  f32 yz = y * z2;

  f32 wx = w * x2;
  f32 wy = w * y2;
  f32 wz = w * z2;

  result.column0 = (__v4f32){1.0f - yy - zz, xy + wz, xz - wy, 0.0f};
  result.column1 = (__v4f32){xy - wz, 1.0f - xx - zz, yz + wx, 0.0f};
  result.column2 = (__v4f32){xz + wy, yz - wx, 1.0f - xx - yy, 0.0f};

  return result;
}
