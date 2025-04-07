/* Jay operating system layer.
 * Open a file, get its size, etc.
 * Implemented for Win32 and Posix.
 * Threads are in jthread.c */
#include "jos.h"

#if _WIN32
#include "windows.h"
#else
#include "fcntl.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"
#include <errno.h>
#endif

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif

/* TODO: Test on old Windows, and possibly improve the code there. */
#if _WIN32
jbool jos_vista_or_newer = 1;
#endif

void jos_set_vista_or_newer(jbool value) {
#if _WIN32
  jos_vista_or_newer = value;
#endif
}

void jos_open_file(jos_file_open_t *self)
/* open a file using relatively low level operating system functionality.
 * Implemented is provided for Win32 and Posix (missing environments include NT
 * kernel and EFI). File can be opened for read or write. This is an internal
 * function. Public functions are: jos_open_file_read and jos_open_file_write.
 */
{
  /* TODO: */
#if _WIN32
  HANDLE f = CreateFileA(self->file_path,
                         GENERIC_READ | (self->read ? 0 : GENERIC_WRITE),
                         FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                         0, self->read ? OPEN_EXISTING : OPEN_ALWAYS, 0, 0);
  if (f == INVALID_HANDLE_VALUE) {
    self->file_handle = -1;
    self->err = GetLastError();
    return;
  }
#else
  int f = open(self->file_path,
               O_CLOEXEC
#ifdef O_CLOFORK
                   | O_CLOFORK
#endif
                   | (self->read ? O_RDONLY : (O_RDWR | O_CREAT)),
               0);
  if (f < 0) {
    self->file_handle = -1;
    self->err = errno;
    return;
  }
#endif
  /* Windows handles are actually only 29 or 30 bits depending on how you count.
   * Handles are interchangable between 32bit and 64 processes.
   * The lower 2 bits are reserved for user
   * kernel handles have upper bit set
  user mode handlers have upper bit clear */
  self->file_handle = (int)(intptr_t)f;
}

int jos_open_file_read(const char *file_path, int *file_handle)
/* open a file using relatively low level operating system functionality. For
 * read. Return the handle through an out parameter and return 0 for success,
 * else error. */
{
  jos_file_open_t self = {0};
  self.read = 1;
  self.file_path = file_path;
  jos_open_file(&self);
  if (!self.err)
    *file_handle = self.file_handle;
  return self.err;
}

int jos_open_file_write(const char *file_path, int *file_handle)
/* open a file using relatively low level operating system functionality. For
 * write. Return the handle through an out parameter and return 0 for success,
 * else error. */
{
  jos_file_open_t self = {0};
  self.file_path = file_path;
  jos_open_file(&self);
  if (!self.err)
    *file_handle = self.file_handle;
  return self.err;
}

void jos_close_file(int file_handle)
/* Close a file. */
{
#if _WIN32
  HANDLE hfile = (HANDLE)(intptr_t)file_handle;
  CloseHandle(hfile);
#else
  close(file_handle);
#endif
}

int jos_set_file_size(int file_handle, int64_t file_size)
/* Set the size of an open file. */
{
#if _WIN32
  LARGE_INTEGER li;
  int err;
  HANDLE hfile = (HANDLE)(intptr_t)file_handle;
  if (jos_vista_or_newer) {
    return SetFileInformationByHandle(hfile, FileEndOfFileInfo, &file_size,
                                      sizeof(file_size))
               ? 0
               : GetLastError();
  }
  li.QuadPart = file_size - 1;
  li.LowPart = SetFilePointer(hfile, li.LowPart, &li.HighPart, 0);
  if (li.LowPart == INVALID_SET_FILE_POINTER) {
    err = GetLastError();
    if (err)
      return err;
  }
  return SetEndOfFile(hfile) ? 0 : GetLastError();
#else
  return ftruncate(file_handle, file_size) ? errno : 0;
#endif
}

int jos_get_file_size(int file_handle, int64_t *file_size)
/* Get the size of an open file. */
{
#if _WIN32
  LARGE_INTEGER li;
  BY_HANDLE_FILE_INFORMATION info = {0};
  HANDLE hfile = (HANDLE)(intptr_t)file_handle;
  if (!GetFileInformationByHandle(hfile, &info))
    return GetLastError();
  li.HighPart = info.nFileSizeHigh;
  li.LowPart = info.nFileSizeLow;
  *file_size = li.QuadPart;
#else
  struct stat st = {0};
  if (fstat(file_handle, &st))
    return errno;
  *file_size = st.st_size;
#endif
  return 0;
}

int jos_mmap(int file_handle, int64_t size, int read, void **q)
/* mmap a file for read or read/write */
{
  void *p;
#if _WIN32
  int err = 0;
  HANDLE hfile = (HANDLE)(intptr_t)file_handle;
  LARGE_INTEGER li;
  HANDLE fileMapping;
  li.QuadPart = size;
  fileMapping =
      CreateFileMappingA(hfile, 0, read ? PAGE_READONLY : PAGE_READWRITE,
                         li.HighPart, li.LowPart, 0);
  if (!fileMapping)
    return GetLastError();
  p = MapViewOfFile(fileMapping, FILE_MAP_READ | (read ? 0 : FILE_MAP_WRITE), 0,
                    0, size);
  if (p)
    *q = p;
  else
    err = GetLastError();
  CloseHandle(fileMapping);
#else
  p = mmap(0 /*addr*/, size, PROT_READ | (read ? 0 : PROT_WRITE),
           read ? MAP_PRIVATE : MAP_SHARED, file_handle, 0 /*offset*/);
  if (p == MAP_FAILED)
    return errno;
  *q = p;
#endif
  return 0;
}

/* int jos_read(int file_handle, void* buffer, size_t bytes, size_t *actual);
 * int jos_write(int file_handle, void* buffer, size_t bytes, size_t *actual);
 */

int jos_mmap_read(int file_handle, int64_t size, void **q)
/* mmap a file for read */
{
  return jos_mmap(file_handle, size, 1, q);
}

int jos_mmap_write(int file_handle, int64_t size, void **q)
/* mmap a file for read/write */
{
  return jos_mmap(file_handle, size, 0, q);
}

int jos_munmap(void *p, size_t size)
/* munmap a file */
{
  if (!p)
    return 0;
#if _WIN32
  UNREFERENCED_PARAMETER(size);
  if (!UnmapViewOfFile(p))
    return GetLastError();
#else
  if (!munmap(p, size))
    return errno;
#endif
  return 0;
}
