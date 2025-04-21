#include "jcommon.h"
#include "jstdio_file.h"

int jstdio_file_get_size(jfile_t *self, size_t *size)
{
  return -1;
}

int jstdio_file_read(jfile_t *self, size_t offset, void *buf, size_t requested,
            size_t *actual)
{

}
int jstdio_file_write(jfile_t *self, size_t offset, void *buf, size_t requested,
                size_t *actual)
{
    return -1;
}

int jstdio_file_close(jfile_t *self)
{
}

int jstdio_file_open(jstdio_file_t * file, const char* path, const char *mode)
{
    file->base.read = jstdio_file_read;
    file->base.close = jstdio_file_close;
    if (!(file->file = fopen(path, mode)))
        return -errno;
    return 0;
}

