#ifndef JARRAY_H
#define JARRAY_H

#include "jbase.h"
#include "jcharp.h"
#include "jrtype.h"

struct jarray_iter;
typedef struct jarray_iter jarray_iter;
struct jarrayt;
typedef struct jarrayt jarrayt;
struct jarray_t typedef struct jarray_t jarray_t;

struct jarray_iter {
  Long pos;
  int (*eq)(jarray_iter *);
  void (*inc)(jarray_iter *);
  jvoidp (*get)(jarray_iter *);
};

struct jarrayt {
  jrtype t;
  void (*begin)(jarray_iter *);
  void (*end)(jarray_iter *);
  void (*push_back)(jvoidp);
  void (*pop_back)(jvoidp);
};

struct jarray_t {
  jarrayt *t;
  jrtype *telem;
  jcharp begin, end, cap;
  Long size;
  Long capacity;
};

int jarray_init(jarray_t *);

#endif
