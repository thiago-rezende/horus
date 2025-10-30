#include <errno.h>
#include <stdio.h>
#include <string.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

struct __platform_file {
  FILE *handle;
};

platform_file_t *platform_file_open(char *path) {
  platform_file_t *file = platform_memory_allocate(sizeof(platform_file_t));

  file->handle = fopen(path, "rb");

  if (file->handle == NULL) {
    logger_critical_format("<path:%s> <code:%d> file opening failed", path, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    platform_memory_deallocate(file);

    return NULL;
  }

  return file;
}

b8 platform_file_close(platform_file_t *file) {
  fclose(file->handle);

  platform_memory_deallocate(file);

  return true;
}

u64 platform_file_size(platform_file_t *file) {
  if (fseek(file->handle, 0, SEEK_END) != 0) {
    logger_critical_format("<file:%p> <handle:%p> <code:%d> file seeking failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    return 0;
  }

  long size_in_bytes = ftell(file->handle);

  if (size_in_bytes == -1L && errno != 0) {
    logger_critical_format("<file:%p> <handle:%p> <code:%d> file size retrieval failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    return 0;
  }

  if (fseek(file->handle, 0, SEEK_SET) != 0) {
    logger_critical_format("<file:%p> <handle:%p> <code:%d> file seeking failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    return 0;
  }

  return (u64)size_in_bytes;
}

u64 platform_file_read(platform_file_t *file, u8 *buffer, u64 size) {
  size_t bytes_read = fread(buffer, sizeof(u8), (size_t)size, file->handle);

  if (bytes_read != size) {
    logger_critical_format("<file:%p> <handle:%p> <code:%d> file reading failed", file, file->handle, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    return 0;
  }

  return (u64)bytes_read;
}
