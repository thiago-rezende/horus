#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

struct __platform_file {
  int descriptor;
};

platform_file_t *platform_file_open(char *path) {
  platform_file_t *file = platform_memory_allocate(sizeof(platform_file_t));

  file->descriptor = open(path, O_RDONLY);

  if (file->descriptor == -1) {
    logger_critical_format("<path:%s> <code:%lu> file opening failed", path, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    platform_memory_deallocate(file);

    return NULL;
  }

  return file;
}

b8 platform_file_close(platform_file_t *file) {
  platform_memory_deallocate(file);

  return true;
}

u64 platform_file_size(platform_file_t *file) {
  struct stat information;

  if (fstat(file->descriptor, &information) == -1) {
    logger_critical_format("<file:%p> <descriptor:%d> <code:%lu> file information retrieval failed", file,
                           file->descriptor, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    return 0;
  }

  return (u64)information.st_size;
}

u64 platform_file_read(platform_file_t *file, u8 *buffer, u64 size) {
  ssize_t bytes_read = read(file->descriptor, buffer, size);

  if (bytes_read == -1) {
    logger_critical_format("<file:%p> <descriptor:%d> <code:%lu> file reading failed", file, file->descriptor, errno);
    logger_critical_format("|- [ error ] %s", strerror(errno));

    return 0;
  }

  return (u64)bytes_read;
}
