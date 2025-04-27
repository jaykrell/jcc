#if !JFILE_UNGET_H
#define JFILE_UNGET_H 1

#include "jfile.h"

typedef struct jfile_unget_t jfile_unget_t;

struct jfile_unget_t {
  jfile_t base;
  jfile_t* next;
  /* in future the size should be dynamic */
  char buf;
  int size;
};

int jfile_unget(jfile_t *file, char ch);

int jfile_unget_init(jfile_unget_t *unget, jfile_t* next);

#endif
