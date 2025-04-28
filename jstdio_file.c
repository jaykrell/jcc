#include "jstdio_file.h"
#include "jbase.h"
#include "jcommon.h"

int jstdio_file_get_size(jfile_t *self, size_t *size) { return -1; }

int jstdio_file_read(jfile_t *self, void *buf, size_t requested,
                     size_t *pactual) {
  size_t actual;
  jstdio_file_t *file = JBASE(jstdio_file_t, base, self);
  *pactual = 0;
  if (!file->file)
    return -EINVAL;
  actual = fread(buf, 1, requested, file->file);
  *pactual = actual;
  if (actual == requested)
    return 0;
  self->eof = feof(file->file);
  if (self->eof)
    return 0;
  return ((self->err = errno));
}

int jstdio_file_close(jfile_t *self) {
  jstdio_file_t *file = JBASE(jstdio_file_t, base, self);
  if (file->file) {
    fclose(file->file);
    file->file = 0;
  }
  return 0;
}

int jstdio_file_open(jstdio_file_t *file, const char *path, const char *mode) {
  file->base.read = jstdio_file_read;
  file->base.close = jstdio_file_close;
  if (!(file->file = fopen(path, mode)))
    return -errno;
  return 0;
}
