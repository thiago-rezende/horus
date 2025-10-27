#pragma once

/* horus core layer */
#include <horus/core/types.h>

/* horus math layer */
#include <horus/math/matrix.h>
#include <horus/math/vector.h>
#include <horus/math/quaternion.h>

typedef enum __camera_type {
  CAMERA_TYPE_NONE = 0,
  CAMERA_TYPE_FIXED,
  CAMERA_TYPE_SPLINE,
  CAMERA_TYPE_FREE_LOOK,
  CAMERA_TYPE_ORTHOGONAL,
  CAMERA_TYPE_FIRST_PERSON,
  CAMERA_TYPE_THIRD_PERSON,
  CAMERA_TYPE_COUNT,
} camera_type_t;

typedef enum __camera_projection {
  CAMERA_PROJECTION_NONE = 0,
  CAMERA_PROJECTION_PERSPECTIVE,
  CAMERA_PROJECTION_ORTHOGRAPHIC,
  CAMERA_PROJECTION_COUNT,
} camera_projection_t;

typedef struct __camera_create_info {
  /* general info */
  camera_type_t type;
  camera_projection_t projection;

  /* movement info */
  f32 speed;

  /* position info */
  vector3f32_t target;
  vector3f32_t position;

  /* rotation info */
  quaternionf32_t rotation;

  /* frustum info */
  f32 far_plane;
  f32 near_plane;
  f32 field_of_view;

  /* projection info */
  u16 width;
  u16 height;
} camera_create_info_t;

typedef struct __camera_update_info {
  /* general info */
  f64 timestep;

  /* position info */
  vector3f32_t target;

  /* projection info */
  u16 width;
  u16 height;
} camera_update_info_t;

typedef struct __camera {
  camera_type_t type;
  camera_projection_t projection;

  f32 speed;

  matrix4f32_t view_matrix;
  matrix4f32_t projection_matrix;

  vector3f32_t up;
  vector3f32_t target;
  vector3f32_t position;

  quaternionf32_t rotation;

  f32 far_plane;
  f32 near_plane;
  f32 field_of_view;

  u16 width;
  u16 height;
} camera_t;

camera_t *camera_create(camera_create_info_t info);
b8 camera_destroy(camera_t *camera);

b8 camera_update(camera_t *camera, camera_update_info_t info);

const char *camera_type_string(camera_type_t type);
const char *camera_projection_string(camera_projection_t projection);
