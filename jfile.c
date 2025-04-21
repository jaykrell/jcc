
#include "jfile.h"

int jfile_read(jfile_t *file,void *buf, size_t requested,
               size_t *actual)
{
    *actual = 0;
    if (file && file->read)
      return file->read(file, buf, requested, actual);
    return -1;
}

void jfile_close(jfile_t *file)
{
    if (file && file->close)
      file->close(file);
}
