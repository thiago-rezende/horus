#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <horus/logger/logger.h>
#include <horus/platform/memory.h>
#include <horus/platform/console.h>

static const char* logger_levels[LOGGER_LEVEL_COUNT] = {
    "[ critical ] ", "[ error    ] ", "[ warning  ] ",
    "[ info     ] ", "[ debug    ] ", "[ trace    ] ",
};

static const u8 level_size = 13;
static const u16 buffer_size = 1024;
static const u32 total_buffer_size = level_size + buffer_size;

void logger(logger_level level, const char* message, ...) {
  va_list args;

  char buffer[total_buffer_size];
  platform_memory_zero(buffer, total_buffer_size);

  strncpy(buffer, logger_levels[level], level_size);

  va_start(args, message);

  vsnprintf(buffer + level_size, buffer_size, message, args);

  va_end(args);

  if (level <= LOGGER_LEVEL_ERROR) {
    platform_console_write_error(buffer);
  } else {
    platform_console_write(buffer);
  }
}
