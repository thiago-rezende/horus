#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <horus/logger/ansi.h>
#include <horus/logger/logger.h>
#include <horus/platform/memory.h>
#include <horus/platform/console.h>

static const char* logger_levels[LOGGER_LEVEL_COUNT] = {
    "[ " ansi_magenta "critical" ansi_reset " ] ",
    "[ " ansi_red "error" ansi_reset "    ] ",
    "[ " ansi_yellow "warning" ansi_reset "  ] ",
    "[ " ansi_green "info" ansi_reset "     ] ",
    "[ " ansi_blue "debug" ansi_reset "    ] ",
    "[ " ansi_grey "trace" ansi_reset "    ] ",
};

static const u8 level_size = 13 + ansi_size * 2;
static const u16 buffer_size = 1024;
static const u32 total_buffer_size = level_size + buffer_size;

void logger(logger_level level, const char* message, ...) {
  va_list args;

  char buffer[total_buffer_size];
  platform_memory_clear(buffer, total_buffer_size);

  strncpy(buffer, logger_levels[level], level_size);

  va_start(args, message);

  i64 written = vsnprintf(buffer + level_size, buffer_size, message, args);

  va_end(args);

  written += level_size;

  buffer[written < total_buffer_size ? written : total_buffer_size - 1] = '\n';

  if (level <= LOGGER_LEVEL_ERROR) {
    platform_console_write_error(buffer);
  } else {
    platform_console_write(buffer);
  }
}
