#include <errno.h>
#include <stdio.h>
#include <string.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

#define FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE 255

struct __platform_file {
  FILE *handle;
};

platform_file_t *platform_file_open(char *path) {
  platform_file_t *file = platform_memory_allocate(sizeof(platform_file_t));

  const errno_t error = fopen_s(&file->handle, path, "rb");

  if (file->handle == NULL) {
    char buffer[FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE] = {0};

    (void)strerror_s(buffer, FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE, error);

    logger_critical_format("<path:%s> <code:%d> file opening failed", path, error);
    logger_critical_format("|- [ error ] %s", buffer);

    platform_memory_deallocate(file);

    return NULL;
  }

  return file;
}

b8 platform_file_close(platform_file_t *file) {
  (void)fclose(file->handle);

  platform_memory_deallocate(file);

  return true;
}

u64 platform_file_size(platform_file_t *file) {
  if (fseek(file->handle, 0, SEEK_END) != 0) {
    char buffer[FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE] = {0};

    (void)strerror_s(buffer, FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE, errno);

    logger_critical_format("<file:%p> <handle:%p> <code:%d> file seeking failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", buffer);

    return 0;
  }

  long size_in_bytes = ftell(file->handle);

  if (size_in_bytes == -1L && errno != 0) {
    char buffer[FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE] = {0};

    (void)strerror_s(buffer, FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE, errno);

    logger_critical_format("<file:%p> <handle:%p> <code:%d> file size retrieval failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", buffer);

    return 0;
  }

  if (fseek(file->handle, 0, SEEK_SET) != 0) {
    char buffer[FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE] = {0};

    (void)strerror_s(buffer, FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE, errno);

    logger_critical_format("<file:%p> <handle:%p> <code:%d> file seeking failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", buffer);

    return 0;
  }

  return (u64)size_in_bytes;
}

u64 platform_file_read(platform_file_t *file, u8 *buffer, u64 size) {
  size_t bytes_read = fread(buffer, sizeof(u8), (size_t)size, file->handle);

  if (bytes_read != size) {
    char buffer[FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE] = {0};

    (void)strerror_s(buffer, FILESYSTEM_ERROR_MESSAGE_BUFFER_SIZE, errno);

    logger_critical_format("<file:%p> <handle:%p> <code:%d> file reading failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", buffer);

    return 0;
  }

  return (u64)bytes_read;
}
