#ifndef JSTDIO_T_H
#define JSTDIO_T_H 1

#include <stdio.h>

struct jstdio_t;
typedef struct jstdio_t jstdio_t;

struct jstdio_t {
	FILE* (*fopen)(jstdio_t* self, const char* path, const char *mode);
	size_t (*fread)(jstdio_t* self, void* buffer, size_t size, size_t count, FILE* file);
	void (*fclose)(jstdio_t* self, FILE* file);
};

extern const jstdio_t jstdio_default;

#endif
