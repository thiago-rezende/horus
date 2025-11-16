/* horus core layer */
#include <horus/core/hash.h>

u64 hash_fnv1a64(const void *data, u64 length) {
  u64 hash = HASH_FNV1A64_OFFSET_BASIS;

  const u8 *bytes = (const u8 *)data;

  for (u64 i = 0; i < length; i++) {
    hash ^= bytes[i];
    hash *= HASH_FNV1A64_PRIME;
  }

  return hash;
}
