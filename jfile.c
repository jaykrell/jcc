/* Jay Krell stuff */
#include "jfile.h"
#include "jmin.h"
#include <string.h>

static int jfile_flush(jfile_t *f) {
  if (!f->buffer_mode)
    return 0;
  return 0;
}

static int jfile_set_buffer_mode(jfile_t *f, int mode) {
  int err = 0;
  if (f->buffer_mode != mode && ((err = jfile_flush(f))))
    return err;
  f->buffer_mode = mode;
  return 0;
}

int jfile_unget(jfile_t *file, int ch) {
  int err = 0;
  if ((err = jfile_set_buffer_mode(file, JFILE_MODE_READ)))
    return err;
  if (file->unget_valid)
    return -1;
  file->unget = ch;
  file->unget_valid = 1;
  return 0;
}

int jfile_read(jfile_t *f, void *buf, size_t requested, size_t *actual) {
  size_t a = 0;
  int err = 0;
  jfile_buffer_t *fb = &f->buffer;
  *actual = 0;
  if ((err = jfile_set_buffer_mode(f, JFILE_MODE_READ)))
    return err;
  if (requested < 1)
    return 0;
  while (f->unget_valid || fb->size) {
    /* Read from unget first. */
    if (f->unget_valid) {
      *(char *)buf = f->unget;
      buf = 1 + (char *)buf;
      f->unget_valid = 0;
      requested -= 1;
      *actual = 1;
      if (requested < 1)
        return 0;
    }
    /* Read from buffer. */
    a = JMIN(fb->size, requested);
    memmove(buf, fb->data + fb->capacity - fb->size, a);
    buf = a + (char *)buf;
    requested -= a;
    f->buffer.size -= a;
    *actual += a;
    if (requested < 1)
      return 0;
    if (err)
      return err;
    /* Fill buffer. */
    /* Hypothetically, read could sorta recurse and unget. */
    err = f->read(f, fb->data, fb->capacity, &fb->size);
  }
  if (err)
    return err;
  return f->read(f, buf, requested, actual);
}

void jfile_close(jfile_t *f) {
  if (f && f->close)
    f->close(f);
}
