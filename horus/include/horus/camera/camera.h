#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/matrix.h>
#include <horus/math/vector.h>
#include <horus/math/quaternion.h>

typedef struct __camera {
  matrix4f32_t view_matrix;
  matrix4f32_t projection_matrix;

  vector3f32_t position;
  quaternionf32_t rotation;

  b8 should_update_view_matrix;
} camera_t;

camera_t *camera_create(vector3f32_t position, quaternionf32_t rotation);
b8 camera_destroy(camera_t *camera);

b8 camera_get_position(camera_t *camera, vector3f32_t *position);
b8 camera_set_position(camera_t *camera, vector3f32_t position);

b8 camera_get_rotation(camera_t *camera, quaternionf32_t *rotation);
b8 camera_set_rotation(camera_t *camera, quaternionf32_t rotation);

b8 camera_look_at(camera_t *camera, vector3f32_t target, vector3f32_t up);
b8 camera_rotate_euler(camera_t *camera, vector3f32_t angles);

matrix4f32_t camera_view_matrix(camera_t *camera);

vector3f32_t camera_up_vector(camera_t *camera);
vector3f32_t camera_down_vector(camera_t *camera);
vector3f32_t camera_left_vector(camera_t *camera);
vector3f32_t camera_right_vector(camera_t *camera);
vector3f32_t camera_forward_vector(camera_t *camera);
vector3f32_t camera_backward_vector(camera_t *camera);

b8 camera_move_up(camera_t *camera, f32 amount);
b8 camera_move_down(camera_t *camera, f32 amount);
b8 camera_move_left(camera_t *camera, f32 amount);
b8 camera_move_right(camera_t *camera, f32 amount);
b8 camera_move_forward(camera_t *camera, f32 amount);
b8 camera_move_backward(camera_t *camera, f32 amount);
