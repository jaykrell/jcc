#if !JSTDIO_H
#define JSTDIO_H 1

#include <stdio.h>

struct jstdio_t;
typedef struct jstdio_t jstdio_t;

struct jstdio_t {
  FILE *(*fopen)(jstdio_t *self, const char *path, const char *mode);
  size_t (*fread)(jstdio_t *self, void *buffer, size_t size, size_t count,
                  FILE *file);
  void (*fclose)(jstdio_t *self, FILE *file);
};

#if __cplusplus
extern "C" {
#endif

extern jstdio_t jstdio_default;

#if __cplusplus
}
#endif

#endif
