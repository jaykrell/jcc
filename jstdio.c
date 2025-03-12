#define _CRT_SECURE_NO_WARNINGS 1 /* fopen is supposedly unsafe */
#include "jstdio.h"
#include <stdio.h>

#pragma warning(disable:4100) /* unused parameter */

FILE* jstdio_fopen(jstdio_t* self, const char* path, const char *mode)
{
	return fopen(path, mode);
}

size_t jstdio_fread(jstdio_t* self, void* buffer, size_t size, size_t count, FILE* file)
{
	return fread(buffer, size, count, file);
}

void jstdio_fclose(jstdio_t* self, FILE* file)
{
	fclose(file);
}

extern const jstdio_t jstdio_default = {
	jstdio_fopen,
	jstdio_fread,
	jstdio_fclose,
};
