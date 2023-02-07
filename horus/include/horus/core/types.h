#pragma once

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
static const i8 max_i8 = 127;
static const i16 max_i16 = 32767;
static const i32 max_i32 = 2147483647;
static const i64 max_i64 = 9223372036854775807;
static const i8 min_i8 = -127 - 1;
static const i16 min_i16 = -32767 - 1;
static const i32 min_i32 = -2147483647 - 1;
static const i64 min_i64 = -9223372036854775807 - 1;

/* unsigned constants */
static const u8 max_u8 = 0xFF;
static const u16 max_u16 = 0xFFFF;
static const u32 max_u32 = 0xFFFFFFFF;
static const u64 max_u64 = 0xFFFFFFFFFFFFFFFF;

/* floating point constants */
static const f32 max_f32 = 3.402823466e+38f;
static const f32 min_f32 = -3.402823466e+38f;
static const f32 smallest_positive_f32 = 1.1754943508e-38f;
static const f32 epsilon_f32 = 5.96046448e-8f;
static const f32 pi_f32 = 3.14159265359f;
static const f32 half_pi_f32 = 1.5707963267f;
static const f64 max_f64 = 1.79769313486231e+308;
static const f64 min_f64 = -1.79769313486231e+308;
static const f64 smallest_positive_f64 = 4.94065645841247e-324;
static const f64 epsilon_f64 = 1.11022302462515650e-16;
