#include <horus/core/strings.h>

u64 string_length(char *string) {
  u64 length = 0;

  while (*string) {
    length++;
    string++;
  }

  return length;
}

u64 string_length_secure(char *string, u64 size) {
  u64 length = 0;

  while (*string) {
    if (length >= size) {
      break;
    }

    length++;
    string++;
  }

  return length;
}

b8 string_compare(char *string_a, char *string_b) {
  while (*string_a && *string_b) {
    if (*string_a != *string_b) {
      return false;
    }

    string_a++;
    string_b++;
  }

  return true;
}

b8 string_compare_secure(char *string_a, char *string_b, u64 size) {
  u64 length = 0;

  while (*string_a && *string_b) {
    if (length >= size) {
      return true;
    }

    if (*string_a != *string_b) {
      return false;
    }

    length++;
    string_a++;
    string_b++;
  }

  return true;
}
