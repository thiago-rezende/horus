#pragma once

/* horus core layer */
#include <horus/core/types.h>

void *platform_memory_allocate(u64 size);

void *platform_memory_reallocate(void *data, u64 size);

b8 platform_memory_deallocate(void *data);

b8 platform_memory_clear(void *data, u64 size);

b8 platform_memory_copy(void *target, void *data, u64 size);
