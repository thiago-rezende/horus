#pragma once

#include <float.h>
#include <stddef.h>
#include <stdint.h>

/* signed types */
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

/* unsigned types */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/* floating point types */
typedef float f32;
typedef double f64;

/* boolean types */
typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

#define true 1
#define false 0

/* signed constants */
static const i8 max_i8 = INT8_MAX;
static const i16 max_i16 = INT16_MAX;
static const i32 max_i32 = INT32_MAX;
static const i64 max_i64 = INT64_MAX;

static const i8 min_i8 = INT8_MIN;
static const i16 min_i16 = INT16_MIN;
static const i32 min_i32 = INT32_MIN;
static const i64 min_i64 = INT64_MIN;

/* unsigned constants */
static const u8 max_u8 = UINT8_MAX;
static const u16 max_u16 = UINT16_MAX;
static const u32 max_u32 = UINT32_MAX;
static const u64 max_u64 = UINT64_MAX;

static const u8 min_u8 = 0;
static const u16 min_u16 = 0;
static const u32 min_u32 = 0;
static const u64 min_u64 = 0;

/* floating point constants */
static const f32 max_f32 = FLT_MAX;
static const f64 max_f64 = DBL_MAX;

static const f32 min_f32 = FLT_MIN;
static const f64 min_f64 = DBL_MIN;

static const f32 epsilon_f32 = FLT_EPSILON;
static const f64 epsilon_f64 = DBL_EPSILON;

/* mathematical constants */

static const f32 pi_f32 = 3.14159265359f;
static const f32 half_pi_f32 = 1.5707963267f;
