#include "jfile.h"
#include <string.h>
#include "jmin.h"

int jfile_flush(jfile_t *file)
{
  if (!file->buffer_mode) return 0;
  return 0;
}

int jfile_set_buffer_mode(jfile_t* file, int mode)
{
  int err=0;
  if (file->buffer_mode != mode && ((err=jfile_flush(file)))) return err;
  file->buffer_mode = mode;
  return 0;
}

int jfile_unget(jfile_t *file, int ch)
{
  int err=0;
  if ((err = jfile_set_buffer_mode(file, JFILE_MODE_READ))) return err;
  if (file->unget_valid)
     return -1;
  file->unget = ch;
  file->unget_valid = 1;
  return 0;
}

int jfile_read(jfile_t *file,void *buf, size_t requested,
               size_t *actual)
{
  size_t a = 0;
  int err=0;
    *actual = 0;
    if ((err = jfile_set_buffer_mode(file, JFILE_MODE_READ))) return err;
    if (requested < 1) return 0;
    while (file->unget_valid || file->buffer.size) {
      /* Read from unget first. */
      if (file->unget_valid) {
        *(char*)buf = file->unget;
        buf = 1 + (char*)buf;
        file->unget_valid = 0;
        requested -= 1;
        *actual = 1;
        if (requested < 1) return 0;
      }
      /* Read from buffer. */
      a = JMIN(file->buffer.size, requested);
      memmove(buf, file->buffer.data + file->buffer.capacity - file->buffer.size, a);
      buf = a + (char*)buf;
      requested -= a;
      file->buffer.size -= a;
      *actual += a;
      if (requested < 1) return 0;
      if (err) return err;
      /* Fill buffer. */
      /* Hypothetically, read could sorta recurse and unget. */
      err = file->read(file, file->buffer.data, file->buffer.capacity, &file->buffer.size);
    }
    if (err) return err;
    return file->read(file, buf, requested, actual);
}

void jfile_close(jfile_t *file)
{
    if (file && file->close)
      file->close(file);
}
