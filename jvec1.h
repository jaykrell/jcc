/* jvec is like std::vector */

#include "jbase.h"
#include "jerr.h"
#include "jpaste.h"
#include "jssize.h"
#include "jtype.h"

#ifndef T

#ifndef JVEC1_H
#define JVEC1_H 1

#define T char
#include "jvec1.h"

#define T int
#include "jvec1.h"

#define T double
#include "jvec1.h"

#endif

#else

#if __cplusplus
extern "C" {
#endif

#define jvec JPASTE(jvec_, T)
#define jvec_fn JPASTE(jvec_fn_, T)

#define jvec_type JPASTE3(jvec_, T, _type)
#define jvec_iter JPASTE3(jvec_, T, _iter)
#define jvec_iter_type JPASTE3(jvec_, T, _iter_type)
#define jvec_init JPASTE3(jvec_, T, _init)

typedef struct jvec jvec; /* a vector, a growable array, like std::vector */
typedef struct jvec_type
    jvec_type; /* the type of a vector, i.e. its functions */
typedef struct jvec_iter jvec_iter; /* an iterator into a vector */
typedef struct jvec_iter_type
    jvec_iter_type; /* the type of an iterator, i.e. its functions */

/* experimental */
#undef JVEC_ITER_TYPE
#define JVEC_ITER_TYPE                                                         \
  jvec_iter (*add_int)(jvec_iter,                                              \
                       ssize_t);    /* add an integer to an iterator */        \
  int (*cmp)(jvec_iter, jvec_iter); /* compare two iterators */                \
  jvec_iter (*dec)(jvec_iter);      /* decrement an iterator */                \
  T *(*get)(jvec_iter);        /* get the data pointed to by the iterator */   \
  jvec_iter (*inc)(jvec_iter); /* increment an interator */                    \
  jvec_iter (*sub_int)(jvec_iter,                                              \
                       ssize_t); /* subtract an integer from an interator */   \
  ssize_t (*sub_iter)(jvec_iter,                                               \
                      jvec_iter); /* subtract an iterator from another */

struct jvec_iter {
  JVEC_ITER_TYPE
  T *p; /* the value of the iterator, a pointer to an element */
};

/* experimental */
#undef JVEC_TYPE
#define JVEC_TYPE                                                              \
  jvec_iter (*beginiter)(jvec *); /* get the start of the vector */            \
  ssize_t (*capacity)(jvec *);    /* get the allocaed end of the vector */     \
  jvec_iter (*enditer)(jvec *);   /* get the logical end+1 of the vector */    \
  jerr (*pop_back)(jvec *);       /* remove the last element of the vector */  \
  jerr (*push_back)(jvec *, T *, ssize_t); /* add elements to the end of the   \
                                            vector, growing it if needed */    \
  jerr (*resize)(jvec *, ssize_t); /* change the logical size of the vector,   \
                                    smaller, larger, or unchanged */           \
  ssize_t (*size)(jvec *);         /* get the logical size of the vector */    \
  jerr (*insert)(jvec *, T *, T *, T *);

struct jvec {
  JVEC_TYPE
  jtype *telem; /* the type of the vector elements */
  T *begin;     /* the start of the vector */
  T *end;       /* the logical end+1 of the vector */
  T *cap;       /* the allocated end+1 of the vector */
};

#if 0
struct jvec_void {
  void *fn;
  void *telem; /* the type of the vector elements */
  void *begin;     /* the start of the vector */
  void *end;       /* the logical end+1 of the vector */
  void *cap;       /* the allocated end+1 of the vector */
};

#define JVEC_SIZE(v) ((v)->begin - (v)->end)
#define JVEC_PUSHBACK(v, elem) (v)->fn.push_back((v), &elem)
#endif

void jvec_init(jvec *v);

#undef T
#undef jvec
#undef jvec_type
#undef jvec_iter
#undef jvec_iter_type
#undef jvec_init

#if __cplusplus
}
#endif

#endif
