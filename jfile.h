#if !JFILE_H
#define JFILE_H 1

/* Rather than speak of abstract "streams" we just call them files.
 * That does not preclude in-memory "files", network "files", directory "files",
 * etc. This follows on the tradition of botj Windows and Unix where
 * "everything" is a file.
 */
#include <stddef.h>
#include <stdint.h>

/* TODO: Hide jfile_t to the providers and implementation? */
typedef struct jfile_t jfile_t;

#define JFILE_MODE_READ 1
#define JFILE_MODE_WRITE 2

typedef struct jbuffer_t {
  char *data;
  size_t size;
  size_t capacity;
  /*  uint64_t file_offset;*/
} jbuffer_t;

struct jfile_t {
  int err;
  char eof;
  char buffer_mode;
  char unget;
  char unget_valid;
  jbuffer_t buffer;
  int (*can_get_file_size)(jfile_t *file);
  int (*can_write)(jfile_t *file);
  int (*get_size)(jfile_t *file, uint64_t *size);
  /*int (*can_seek)(jfile_t* file);
    int (*seek_forward)(jfile_t* file, int64_t);
    int (*seek_backward)(jfile_t* file, int64_t);
    int (*seek_to)(jfile_t* file, int64_t);
    int (*can_mmap)(jfile_t* file); */
  int (*read)(jfile_t *file, void *buf, size_t requested, size_t *actual);
  int (*write)(jfile_t *file, void *buf, size_t requested, size_t *actual);
  int (*read_at)(jfile_t *file, uint64_t file_offset, void *buf,
                 size_t requested, size_t *actual);
  int (*write_at)(jfile_t *file, uint64_t file_offset, void *buf,
                  size_t requested, size_t *actual);
  int (*close)(jfile_t *file);
};

/*int jfile_init(const char *, jfile_t **);*/
int jfile_read(jfile_t *file, void *buf, size_t requested, size_t *actual);
/*
int jfile_unget(jfile_t *file,void *buf, size_t requested,
               size_t *actual);

int jfile_unget_init(jfile_unget_t *unget, jfile_t* file);
*/
#endif
