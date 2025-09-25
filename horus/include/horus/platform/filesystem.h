#pragma once

/* horus core layer */
#include <horus/core/types.h>

typedef struct __platform_file platform_file_t;

platform_file_t *platform_file_open(char *path);
b8 platform_file_close(platform_file_t *file);

u64 platform_file_size(platform_file_t *file);

u64 platform_file_read(platform_file_t *file, u8 *buffer, u64 size);
