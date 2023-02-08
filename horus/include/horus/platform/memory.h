#pragma once

#include <horus/core/types.h>

void* platform_memory_allocate(u64 size);

void* platform_memory_reallocate(void* data, u64 size);

void platform_memory_deallocate(void* data);

void platform_memory_zero(void* data, u64 size);
