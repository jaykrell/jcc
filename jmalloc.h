#ifndef JMALLOC_H
#define JMALLOC_H 1

#include <stddef.h>

struct jmalloc_t;
typedef struct jmalloc_t jmalloc_t;

struct jmalloc_t {
	void* (*malloc)(jmalloc_t* self, size_t n);
	void* (*realloc)(jmalloc_t* self, void* p, size_t n);
	void (*free)(jmalloc_t* self, void*);
};

extern jmalloc_t jmalloc_default;

#endif
