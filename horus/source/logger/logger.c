#include <stdio.h>
#include <stdarg.h>

/* horus logger layer */
#include <horus/logger/ansi.h>
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/console.h>

static const char *logger_prefixes[LOGGER_LEVEL_COUNT] = {
    [LOGGER_LEVEL_CRITICAL] = "[ " ansi_magenta "critical" ansi_reset " ] ",
    [LOGGER_LEVEL_ERROR] = "[ " ansi_red "error" ansi_reset "    ] ",
    [LOGGER_LEVEL_WARNING] = "[ " ansi_yellow "warning" ansi_reset "  ] ",
    [LOGGER_LEVEL_INFO] = "[ " ansi_green "info" ansi_reset "     ] ",
    [LOGGER_LEVEL_TRACE] = "[ " ansi_white "trace" ansi_reset "    ] ",
    [LOGGER_LEVEL_DEBUG] = "[ " ansi_blue "debug" ansi_reset "    ] ",
};

static const u8 prefix_size = 13 + ansi_size * 2;
static const u16 buffer_size = 1024;
static const u32 total_buffer_size = prefix_size + buffer_size;

b8 __logger_general(logger_level_t level, const char *message, ...) {
  va_list args;

  char buffer[total_buffer_size];
  platform_memory_clear(buffer, total_buffer_size);

  platform_memory_copy(buffer, (void *)logger_prefixes[level], prefix_size);

  va_start(args, message);

  i64 written = vsnprintf(buffer + prefix_size, buffer_size, message, args);

  va_end(args);

  written += prefix_size;

  buffer[written < total_buffer_size ? written : total_buffer_size - 1] = '\n';

  if (level <= LOGGER_LEVEL_ERROR) {
    return platform_console_write_error(buffer);
  }

  return platform_console_write(buffer);
}
