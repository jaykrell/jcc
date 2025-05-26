/* Jay Krell stuff */
#include "jfile.h"
#include "jmin.h"
#include <string.h>

int jfile_flush(jfile_t *f)
/* flush buffer for close or switching from read to write */
{
  if (!f->buffer_mode)
    return 0;
  return 0;
}

static int jfile_set_buffer_mode(jfile_t *f, int mode)
/* switch buffer from read to write or vice versa */
{
  int err = 0;
  if (f->buffer_mode != mode && ((err = jfile_flush(f))))
    return err;
  f->buffer_mode = (char)mode;
  return 0;
}

int jfile_unget(jfile_t *f, char ch) {
  int err = 0;
  if ((err = jfile_set_buffer_mode(f, JFILE_MODE_READ)))
    return err;
  if (f->unget_valid)
    return (f->err = -1);
  f->unget = ch;
  f->unget_valid = 1;
  return 0;
}

int jfile_read(jfile_t *f, void *buf, size_t requested, size_t *actual) {
  size_t a = 0;
  int err = 0;
  jfile_buffer_t *fb = &f->buffer;
  *actual = 0;

  if (requested < 1)
    return 0;

  /* Read from unget first. */
  if (f->unget_valid) {
    *(char *)buf = f->unget;
    buf = 1 + (char *)buf;
    f->unget_valid = 0;
    requested -= 1;
    *actual += 1;
    if (requested < 1)
      return 0;
  }

  /* errors are sticky if not cleared */
  if ((err = f->err))
    return err;

  /* switch buffer from read to write */
  if ((err = jfile_set_buffer_mode(f, JFILE_MODE_READ)))
    return err;

  while (f->unget_valid || fb->size > 0) {
    /* Read from unget first. */
    if (f->unget_valid) {
      *(char *)buf = f->unget;
      buf = 1 + (char *)buf;
      f->unget_valid = 0;
      requested -= 1;
      *actual += 1;
      if (requested < 1)
        return 0;
    }
    /* Read from buffer. */
    a = JMIN(fb->size, requested);
    if (a > 0) {
      memmove(buf, fb->data + fb->capacity - fb->size, a);
      buf = a + (char *)buf;
      requested -= a;
      fb->size -= a;
      *actual += a;
      if (requested < 1)
        return 0;
    }
    if (err)
      return err;
    /* If request larger than buffer and buffer is empty, skip it. */
    if (requested >= fb->capacity)
      return f->err = f->read(f, buf, requested, actual);
    /* Fill buffer. */
    /* Hypothetically, read could sorta recurse and unget. */
    err = f->err = f->read(f, fb->data, fb->capacity, &fb->size);
  }
  if (err)
    return err;
  return (f->err = f->read(f, buf, requested, actual));
}

int jfile_write(jfile_t *f, void *buf, size_t requested, size_t *actual) {
  int err = 0;
  *actual = 0;

  if (requested < 1)
    return 0;

  /* errors are stick if not cleared */
  if ((err = f->err))
    return err;

  /* switch buffer from read to write */
  if ((err = jfile_set_buffer_mode(f, JFILE_MODE_WRITE)))
    return err;
  /* TODO: Buffering */
  return (f->err = f->write(f, buf, requested, actual));
}

int jfile_close(jfile_t *f) {
  if (f && f->close)
    return f->close(f);
  return 0;
}
