#pragma once

/* horus core layer */
#include <horus/core/types.h>

#define HASH_FNV1A64_PRIME 1099511628211ULL
#define HASH_FNV1A64_OFFSET_BASIS 14695981039346656037ULL

u64 hash_fnv1a64(const void *data, u64 length);
