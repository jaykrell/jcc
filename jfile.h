#ifndef JFILE
#define JFILE

typedef struct jfile_t jfile_t;

typedef struct jfile_t {
	int (*get_size) (jlong* size);
	int (*read) ( jfile* self, jlong offset, voidp buf, jlong requested, jlong* actual);
	int (*write) ( jfile* self, jlong offset, voidp buf, jlong requested, jlong* actual);
	int (*close) ( jfile* self);
} jfile_t;

int jfile_init(const char*, jfile_t**);

#endif
