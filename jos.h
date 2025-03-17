/* Jay operating system layer.
 * Open a file, get its size, etc.
 * Implemented for Win32 and Posix.
 * Threads are in jthread.h */
#ifndef JOS_H
#define JOS_H 1

/* aka PAL platform adaption layer */

#include "jbool.h"
#include <stddef.h>
#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

/* All open and CreateFile options shall be represented here. */
typedef struct jos_file_open_t {
  /* basics */
  const char *file_path;
  /* const wchar_t* file_path_w; */
  int file_handle;
  int err;
  int read : 1;
  int write : 1;

  /* fancy */
  int file_share_read : 1;
  int file_share_write : 1;
  int file_share_delete : 1;
  int o_cloexec : 1;
  int o_clofork : 1;
  int o_rdonly : 1;
  int o_rdwr : 1;
  int o_creat : 1;
} jos_file_open_t;

void jos_open_file(jos_file_open_t *args);

int jos_open_file_read(const char *, int *file_handle);
int jos_open_file_write(const char *, int *file_handle);

void jos_close_file(int file_handle);

int jos_read(int file_handle, void *buffer, size_t bytes, size_t *actual);
int jos_write(int file_handle, void *buffer, size_t bytes, size_t *actual);

int jos_set_file_size(int file_handle, int64_t file_size);
;
int jos_get_file_size(int file_handle, int64_t *file_size);

int jos_mmap_read(int file_handle, int64_t size, void **q);
int jos_mmap_write(int file_handle, int64_t size, void **q);
int jos_munmap(void *p, size_t size);

void jos_set_vista_or_newer(jbool);

#if __cplusplus
}
#endif

#endif
