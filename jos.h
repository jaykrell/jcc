#ifndef JOS_H
#define JOS_H 1

/* aka PAL platform adaption layer */

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

/*int jos_error(void);*/
int jos_open_file_read(const char*, int* fd);
int jos_open_file_write(const char*, int* fd);
int jos_set_file_size(int fd, int64_t);
int jos_get_file_size(int fd, int64_t*);
int jos_mmap_read(int fd, int64_t size);
int jos_mmap_write(int fd, int64_t size);

#if __cplusplus
}
#endif

#endif
