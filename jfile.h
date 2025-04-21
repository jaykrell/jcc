#if !JFILE_H
#define JFILE_H 1

/* Rather than speak of abstract "streams" we just call them files.
 * That does not preclude in-memory "files", network "files", directory "files", etc.
 * This follows on the tradition of botj Windows and Unix where
 * "everything" is a file.
 */
#include <stddef.h>

typedef struct jfile_t jfile_t;

struct jfile_t {
  int (*can_get_file_size)(jfile_t* file);
  int (*can_write)(jfile_t* file);
  int (*get_size)(jfile_t *file, uint64_t *size);
/*int (*can_seek)(jfile_t* file);
  int (*seek_forward)(jfile_t* file, int64_t);
  int (*seek_backward)(jfile_t* file, int64_t);
  int (*seek_to)(jfile_t* file, int64_t);
  int (*can_mmap)(jfile_t* file); */
  int (*read)(jfile_t *file, size_t offset, void *buf, size_t requested,
              size_t *actual);
  int (*write)(jfile_t *file, size_t offset, void *buf, size_t requested,
               size_t *actual);
  int (*close)(jfile_t *file);
};

int jfile_init(const char *, jfile_t **);

#endif
