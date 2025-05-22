#pragma once

/* horus core layer */
#include <horus/core/types.h>

u64 string_length(char *string);
u64 string_length_secure(char *string, u64 size);

b8 string_compare(char *string_a, char *string_b);
b8 string_compare_secure(char *string_a, char *string_b, u64 size);
