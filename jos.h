#ifndef JOS_H
#define JOS_H 1

/* aka PAL platform adaption layer */

#include <stdint.h>
#include "jbool.h"

#if __cplusplus
extern "C" {
#endif

int jos_open_file_read(const char*, int* file_handle);
int jos_open_file_write(const char*, int* file_handle);
int jos_set_file_size(int file_handle, int64_t file_size);;
int jos_get_file_size(int file_handle, int64_t* file_size);
int jos_mmap_read(int file_handle, int64_t size, void** q);
int jos_mmap_write(int file_handle, int64_t size, void** q);
void jos_unmap(void*);

void jos_set_vista_or_newer(jbool);


#if __cplusplus
}
#endif

#endif
