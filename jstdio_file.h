#if !JSTDIO_FILE_H
#define JSTDIO_FILE_H

#include <stdio.h>
#include "jfile.h"

typedef struct jstdio_file_t jstdio_file_t;

struct jstdio_file_t
{
    jfile_t base;
    FILE* file;
};

int jstdio_file_open(jstdio_file_t * file, const char* path, const char *mode);

#endif

