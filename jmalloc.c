#include "jmalloc.h"
#include <stdlib.h>

#pragma warning(disable:4100) /* unused parameter */

void* jmalloc_malloc(jmalloc_t* self, size_t n)
{
	return malloc(n);
}

void* jmalloc_realloc(jmalloc_t* self, void* p, size_t n)
{
	return realloc(p, n);
}

void jmalloc_free(jmalloc_t* self, void* p)
{
	free(p);
}

jmalloc_t jmalloc_default = {
	jmalloc_malloc,
	jmalloc_realloc,
	jmalloc_free
};
