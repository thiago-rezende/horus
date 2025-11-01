#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include <math.h>
#include <stdio.h>

#define HORUS_ENTRYPOINT_DISABLE
#include <horus/horus.h>

static void test_matrix4f32_scale(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  vector3f32_t scale = (vector3f32_t){{2, 4, 8}};
  matrix4f32_t identity = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {2, 0, 0, 0},
      .column1 = {0, 4, 0, 0},
      .column2 = {0, 0, 8, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_scale(identity, scale);

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_identity(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_identity();

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_multiply(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  matrix4f32_t a = (matrix4f32_t){
      .column0 = {1, 0, 0, 1},
      .column1 = {0, 2, 2, 0},
      .column2 = {0, 3, 3, 0},
      .column3 = {4, 0, 0, 4},
  };

  matrix4f32_t b = (matrix4f32_t){
      .column0 = {1, 0, 0, 1},
      .column1 = {0, 2, 2, 0},
      .column2 = {0, 3, 3, 0},
      .column3 = {4, 0, 0, 4},
  };

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {5, 0, 0, 5},
      .column1 = {0, 10, 10, 0},
      .column2 = {0, 15, 15, 0},
      .column3 = {20, 0, 0, 20},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_multiply(a, b);

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_translate(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  vector3f32_t vector = (vector3f32_t){{2, 4, 8}};
  matrix4f32_t identity = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {2, 4, 8, 1},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_translate(identity, vector);

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_rotate_euler(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  vector3f32_t angles = (vector3f32_t){{0, 0, 90}};
  matrix4f32_t identity = (matrix4f32_t){
      .column0 = {1, 0, 0, 0},
      .column1 = {0, 1, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* expected result */
  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {0, 1, 0, 0},
      .column1 = {-1, 0, 0, 0},
      .column2 = {0, 0, 1, 0},
      .column3 = {0, 0, 0, 1},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_rotate_euler(identity, angles);

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_perspective(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  f32 aspect_ratio = 1.0f;
  f32 field_of_view = 90.0f;
  f32 near_clip = 0.1f;
  f32 far_clip = 100.0f;

  f32 field_of_view_radians = field_of_view * (pi_f32 / 180.0f);
  f32 tan_half_field_of_view = tan(field_of_view_radians / 2.0f);
  f32 field_of_view_factor = 1.0f / tan_half_field_of_view;
  f32 near_minus_far_inverse = 1.0f / (near_clip - far_clip);

  /* expected result */
  const f32 expected_x_scale = field_of_view_factor / aspect_ratio;
  /* Y-axis flip for Normalized Device Coordinates (NDC) compatibility */
  const f32 expected_y_scale = field_of_view_factor * -1.0f;
  const f32 expected_z_scale = far_clip * near_minus_far_inverse;
  const f32 expected_z_offset = far_clip * near_clip * near_minus_far_inverse;

  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {expected_x_scale, 0, 0, 0},
      .column1 = {0, expected_y_scale, 0, 0},
      .column2 = {0, 0, expected_z_scale, -1},
      .column3 = {0, 0, expected_z_offset, 0},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_perspective(aspect_ratio, field_of_view, near_clip, far_clip);

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

static void test_matrix4f32_orthographic(void **state) {
  (void)state; /* unused */

  /* default attrubutes */
  f32 left = -10.0f;
  f32 right = 10.0f;
  f32 bottom = -5.0f;
  f32 top = 5.0f;
  f32 near_clip = 1.0f;
  f32 far_clip = 10.0f;

  /* expected result */
  f32 inverse_dx = 1.0f / (right - left);
  f32 inverse_dy = 1.0f / (top - bottom);
  f32 near_minus_far_inverse = 1.0f / (near_clip - far_clip);

  const f32 expected_x_scale = 2.0f * inverse_dx;
  /* Y-axis flip for Normalized Device Coordinates (NDC) compatibility */
  const f32 expected_y_scale = 2.0f * inverse_dy * -1.0f;
  const f32 expected_x_translate = -(right + left) * inverse_dx;
  const f32 expected_y_translate = -(top + bottom) * inverse_dy;
  const f32 expected_z_scale = near_minus_far_inverse;
  const f32 expected_z_translate = near_clip * near_minus_far_inverse;

  matrix4f32_t expected = (matrix4f32_t){
      .column0 = {expected_x_scale, 0, 0, expected_x_translate},
      .column1 = {0, expected_y_scale, 0, expected_y_translate},
      .column2 = {0, 0, expected_z_scale, 0.0f},
      .column3 = {0, 0, expected_z_translate, 1.0f},
  };

  /* function call */
  matrix4f32_t result = matrix4f32_orthographic(left, right, bottom, top, near_clip, far_clip);

  /* union assertions */
  assert_float_equal(result.x0, expected.x0, epsilon_f32);
  assert_float_equal(result.y0, expected.y0, epsilon_f32);
  assert_float_equal(result.z0, expected.z0, epsilon_f32);
  assert_float_equal(result.w0, expected.w0, epsilon_f32);

  assert_float_equal(result.x1, expected.x1, epsilon_f32);
  assert_float_equal(result.y1, expected.y1, epsilon_f32);
  assert_float_equal(result.z1, expected.z1, epsilon_f32);
  assert_float_equal(result.w1, expected.w1, epsilon_f32);

  assert_float_equal(result.x2, expected.x2, epsilon_f32);
  assert_float_equal(result.y2, expected.y2, epsilon_f32);
  assert_float_equal(result.z2, expected.z2, epsilon_f32);
  assert_float_equal(result.w2, expected.w2, epsilon_f32);

  assert_float_equal(result.x3, expected.x3, epsilon_f32);
  assert_float_equal(result.y3, expected.y3, epsilon_f32);
  assert_float_equal(result.z3, expected.z3, epsilon_f32);
  assert_float_equal(result.w3, expected.w3, epsilon_f32);
}

int main(void) {
  const struct CMUnitTest tests[] = {
      /* operations */
      cmocka_unit_test(test_matrix4f32_scale),
      cmocka_unit_test(test_matrix4f32_identity),
      cmocka_unit_test(test_matrix4f32_multiply),
      cmocka_unit_test(test_matrix4f32_translate),
      cmocka_unit_test(test_matrix4f32_rotate_euler),
      /* projections */
      cmocka_unit_test(test_matrix4f32_perspective),
      cmocka_unit_test(test_matrix4f32_orthographic),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
