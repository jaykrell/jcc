#include "read_entire_file.h"

void read_entire_file(read_entire_file_t *self) {
  size_t capacity = 0x10000;
  jstdio_t *stdio = self->stdio;
  jmalloc_t *malloc = self->malloc;

  FILE *file = stdio->fopen(stdio, self->file_path, "rb");
  self->size = 0;
  if (!file) {
    self->fopen_failed = 1;
    return;
  }
  if (!(self->contents = (char *)malloc->malloc(malloc, capacity))) {
    self->out_of_memory = 1;
    return;
  }
  while (1) {
    void *new_contents;
    size_t bytes_to_read = (capacity - self->size);
    size_t bytes_read = stdio->fread(stdio, &self->contents[self->size], 1,
                                     bytes_to_read, file);
    self->size += bytes_read;
    if (bytes_read < bytes_to_read) {
      self->contents =
          (char *)malloc->realloc(malloc, self->contents, self->size);
      stdio->fclose(stdio, file);
      return;
    }
    new_contents = malloc->realloc(malloc, self->contents, capacity *= 2);
    if (!new_contents) {
      self->out_of_memory = 1;
      return;
    }
    self->contents = (char *)new_contents;
  }
}
