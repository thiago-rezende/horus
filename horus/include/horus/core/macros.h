#pragma once

#include <horus/core/types.h>

/* memory size macros */
#define B(x) (x)
#define KB(x) ((x) << 10)
#define MB(x) ((x) << 20)
#define GB(x) ((x) << 30)
#define TB(x) (((u64)x) << 40)
