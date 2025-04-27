/* stdio has unget that can unget up to one file character, but not EOF.
 * This provides this functionality into a layerable system of "files".
 */
#include "jfile_unget.h"
#include "jbase.h"
#include "jmin.h"
#include <string.h>

int jfile_unget(jfile_t *file, char ch)
/* Unget into a file. This is modeled after a feature of stdio. */
{
  jfile_unget_t *unget = JBASE(jfile_unget_t, base, file);
  if (unget->size)
    return -1;
  unget->size = 1;
  unget->buf = ch;
  return 0;
}

int jfile_unget_read(jfile_t *file, void *buf, size_t requested, size_t *actual)
/* Read from an unget file. */
{
  size_t n;
  jfile_unget_t *unget = JBASE(jfile_unget_t, base, file);
  if (unget->size > 0) {
    n = JMIN(unget->size, requested);
    memcpy(buf, &unget->buf, n);
    requested -= n;
    buf = n + (char *)buf;
    *actual += n;
    unget->size -= n;
    if (requested == 0)
      return 0;
  }
  return unget->next->read(unget->next, buf, requested, actual);
}

int jfile_unget_close(jfile_t *self)
/* Close an unget file. */
{
  jfile_unget_t *unget = JBASE(jfile_unget_t, base, self);
  return unget->next->close(unget->next);
}

int jfile_unget_init(jfile_unget_t *unget, jfile_t *file)
/* Initialize the unget layer of a file stack. */
{
  unget->base.read = jfile_unget_read;
  unget->base.close = jfile_unget_close;
  unget->next = file;
  unget->size = 0;
  unget->buf = 0;
  return 0;
}
