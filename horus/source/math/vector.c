#include <math.h>

/* horus math layer */
#include <horus/math/vector.h>

/* horus logger layer */
#include <horus/logger/logger.h>

vector4u8_t vector4u8_add(vector4u8_t a, vector4u8_t b) {
  vector4u8_t result;

  result.vector = a.vector + b.vector;

  return result;
}

vector3f32_t vector3f32_up(void) {
  return (vector3f32_t){.x = 0.0f, .y = 1.0f, .z = 0.0f, .__padding = 0.0f};
}

vector3f32_t vector3f32_down(void) {
  return (vector3f32_t){.x = 0.0f, .y = -1.0f, .z = 0.0f, .__padding = 0.0f};
}

vector3f32_t vector3f32_add(vector3f32_t a, vector3f32_t b) {
  vector3f32_t result;

  result.vector = a.vector + b.vector;

  return result;
}

f32 vector3f32_dot(vector3f32_t a, vector3f32_t b) {
  vector3f32_t hadamard = vector3f32_hadamard(a, b);

  f32 result = hadamard.x + hadamard.y + hadamard.z;

  return result;
}

vector3f32_t vector3f32_cross(vector3f32_t a, vector3f32_t b) {
  vector3f32_t result = {{0.0f}};

  result.x = (a.y * b.z) - (a.z * b.y);
  result.y = (a.z * b.x) - (a.x * b.z);
  result.z = (a.x * b.y) - (a.y * b.x);

  return result;
}

vector3f32_t vector3f32_scalar(vector3f32_t vector, f32 scalar) {
  vector3f32_t result;

  result.vector = vector.vector * (__v3f32){scalar, scalar, scalar, 1.0f};

  return result;
}

vector3f32_t vector3f32_hadamard(vector3f32_t a, vector3f32_t b) {
  vector3f32_t result;

  __v3f32 a_vector = (__v3f32){a.x, a.y, a.z, 0.0f};
  __v3f32 b_vector = (__v3f32){b.x, b.y, b.z, 0.0f};

  result.vector = a_vector * b_vector;

  return result;
}

vector3f32_t vector3f32_subtract(vector3f32_t a, vector3f32_t b) {
  vector3f32_t result;

  result.vector = a.vector - b.vector;

  return result;
}

f32 vector3f32_magnitude(vector3f32_t vector) {
  f32 result = 0.0f;
  vector3f32_t squared = {0};

  squared.vector = vector.vector * vector.vector;

  result = sqrtf(squared.x + squared.y + squared.z);

  return result;
}

vector3f32_t vector3f32_normalize(vector3f32_t vector) {
  vector3f32_t result = {0};

  f32 magnitude = vector3f32_magnitude(vector);

  if (magnitude == 0.0f) {
    logger_error("<vector3f32_normalize> zero magnitude");

    return result;
  }

  f32 inverse_magnitude = 1.0f / magnitude;

  result.vector = vector.vector * (__v3f32){inverse_magnitude, inverse_magnitude, inverse_magnitude, inverse_magnitude};

  return result;
}

vector4f32_t vector4f32_add(vector4f32_t a, vector4f32_t b) {
  vector4f32_t result;

  result.vector = a.vector + b.vector;

  return result;
}
