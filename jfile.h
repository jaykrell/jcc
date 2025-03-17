#ifndef JFILE_H
#define JFILE_H 1

#include <stddef.h>

typedef struct jfile_t jfile_t;

struct jfile_t {
  int (*get_size)(size_t *size);
  int (*read)(jfile_t *self, size_t offset, void *buf, size_t requested,
              size_t *actual);
  int (*write)(jfile_t *self, size_t offset, void *buf, size_t requested,
               size_t *actual);
  int (*close)(jfile_t *self);
};

int jfile_init(const char *, jfile_t **);

#endif
