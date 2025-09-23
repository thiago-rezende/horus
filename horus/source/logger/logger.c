#include <stdio.h>
#include <stdarg.h>

/* horus logger layer */
#include <horus/logger/ansi.h>
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/console.h>

static const char *logger_prefix_strings[LOGGER_LEVEL_COUNT] = {
    [LOGGER_LEVEL_CRITICAL] = "[ " ansi_magenta "critical" ansi_reset " ] ",
    [LOGGER_LEVEL_ERROR] = "[ " ansi_red "error" ansi_reset "    ] ",
    [LOGGER_LEVEL_WARNING] = "[ " ansi_yellow "warning" ansi_reset "  ] ",
    [LOGGER_LEVEL_INFO] = "[ " ansi_green "info" ansi_reset "     ] ",
    [LOGGER_LEVEL_TRACE] = "[ " ansi_white "trace" ansi_reset "    ] ",
    [LOGGER_LEVEL_DEBUG] = "[ " ansi_blue "debug" ansi_reset "    ] ",
};

static const u8 logger_prefix_string_size = 13 + ansi_string_size * 2;
static const u16 logger_message_buffer_size = 1024;
static const u32 total_logger_message_buffer_size = logger_prefix_string_size + logger_message_buffer_size;

b8 __logger_general(logger_level_t level, const char *message) {
  char buffer[total_logger_message_buffer_size];

  platform_memory_clear(buffer, total_logger_message_buffer_size);

  platform_memory_copy(buffer, (void *)logger_prefix_strings[level], logger_prefix_string_size);

  i64 written = snprintf(buffer + logger_prefix_string_size, logger_message_buffer_size, "%s", message);

  written += logger_prefix_string_size;

  buffer[written < total_logger_message_buffer_size ? written : total_logger_message_buffer_size - 1] = '\n';

  if (level <= LOGGER_LEVEL_ERROR) {
    return platform_console_write_error(buffer);
  }

  return platform_console_write(buffer);
}

b8 __logger_general_format(logger_level_t level, const char *message, ...) {
  va_list args;

  char buffer[total_logger_message_buffer_size];

  platform_memory_clear(buffer, total_logger_message_buffer_size);

  platform_memory_copy(buffer, (void *)logger_prefix_strings[level], logger_prefix_string_size);

  va_start(args, message);

  i64 written = vsnprintf(buffer + logger_prefix_string_size, logger_message_buffer_size, message, args);

  va_end(args);

  written += logger_prefix_string_size;

  buffer[written < total_logger_message_buffer_size ? written : total_logger_message_buffer_size - 1] = '\n';

  if (level <= LOGGER_LEVEL_ERROR) {
    return platform_console_write_error(buffer);
  }

  return platform_console_write(buffer);
}
