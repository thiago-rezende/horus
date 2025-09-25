/* horus platform layer [ windows ] */
#include <horus/platform/windows/windows.h>

/* horus logger layer */
#include <horus/logger/logger.h>

/* horus platform layer */
#include <horus/platform/memory.h>
#include <horus/platform/filesystem.h>

struct __platform_file {
  HANDLE handle;
};

platform_file_t *platform_file_open(char *path) {
  platform_file_t *file = platform_memory_allocate(sizeof(platform_file_t));

  file->handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (file->handle == INVALID_HANDLE_VALUE) {
    DWORD error = GetLastError();

    logger_critical_format("<path:%s> <code:%lu> file opening failed", path, error);

    platform_memory_deallocate(file);

    return NULL;
  }

  return file;
}

b8 platform_file_close(platform_file_t *file) {
  CloseHandle(file->handle);

  platform_memory_deallocate(file);

  return true;
}

u64 platform_file_size(platform_file_t *file) {
  BY_HANDLE_FILE_INFORMATION information;

  if (!GetFileInformationByHandle(file->handle, &information)) {
    return 0;
  }

  LARGE_INTEGER size;

  size.HighPart = information.nFileSizeHigh;
  size.LowPart = information.nFileSizeLow;

  ULONGLONG size_in_bytes = size.QuadPart;

  return (u64)size_in_bytes;
}

u64 platform_file_read(platform_file_t *file, u8 *buffer, u64 size) {
  DWORD bytes_read = 0;

  if (!ReadFile(file->handle, buffer, size, &bytes_read, NULL)) {
    DWORD error = GetLastError();

    logger_critical_format("<file:%p> <handle:%p> <code:%lu> file reading failed", file, file->handle, error);

    return 0;
  }

  return (u64)bytes_read;
}
