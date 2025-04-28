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

typedef struct jfile_buffer_t {
  char *data;
  size_t size;
  size_t capacity;
  /*  uint64_t file_offset;*/
} jfile_buffer_t;

struct jfile_t {
  int err;  /* Has error occured, and if so, what is it? */
  char eof; /* Has end of file occured, boolean. */
  char buffer_mode; /* Is buffer for reading or writing? */
  char unget; /* Single byte go-back for reading, like stdio. */
  char unget_valid; /* Is unget valid? */
  jfile_buffer_t buffer; /* Large optional buffer. */
  int (*can_get_file_size)(jfile_t *file); /* Can the file's size be queried? */
  int (*can_write)(jfile_t *file); /* Is the file wriable? */
  int (*get_size)(jfile_t *file, uint64_t *size); /* Get the file's size. */
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
int jfile_write(jfile_t *file, void *buf, size_t requested, size_t *actual);
int jfile_unget(jfile_t *file, char ch);
int jfile_flush(jfile_t *file);
int jfile_close(jfile_t* file);
/*
int jfile_unget_init(jfile_unget_t *unget, jfile_t* file);
*/
#endif
