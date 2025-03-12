#ifndef READ_ENTIRE_FILE_H
#define READ_ENTIRE_FILE_H 1

#include "jmalloc.h"
#include "jstdio.h"

typedef struct read_entire_file_t {
	const char *file_path;
	jmalloc_t* malloc;
	jstdio_t*  stdio;
	char* contents;
	size_t size;
	int out_of_memory;
	int fopen_failed;
} read_entire_file_t;

void read_entire_file(read_entire_file_t* self);

#endif
