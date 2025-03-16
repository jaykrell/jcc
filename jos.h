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
