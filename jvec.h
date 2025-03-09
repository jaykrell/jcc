/* jvec is like std::vector */

#include "jbase.h"
#include "jerr.h"
#include "jtype.h"

#ifndef T

#ifndef JVEC_H
#define JVEC_H 1

#define T char
#define jvec           jvec_char
#define jvec_type      jvec_char_type
#define jvec_iter      jvec_char_iter
#define jvec_iter_type jvec_char_iter_type
#define jvec_init      jvec_char_init
#include "jvec.h"

#define T int
#define jvec           jvec_int
#define jvec_type      jvec_int_type
#define jvec_iter      jvec_int_iter
#define jvec_iter_type jvec_int_iter_type
#define jvec_init      jvec_int_init
#include "jvec.h"

#define T double
#define jvec           jvec_double
#define jvec_type      jvec_double_type
#define jvec_iter      jvec_double_iter
#define jvec_iter_type jvec_double_iter_type
#define jvec_init      jvec_double_init
#include "jvec.h"

#endif

#else

typedef struct jvec           jvec;                /* a vector, a growable array, like std::vector */
typedef struct jvec_type      jvec_type;           /* the type of a vector, i.e. its functions */
typedef struct jvec_iter      jvec_iter;           /* an iterator into a vector */
typedef struct jvec_iter_type jvec_iter_type;      /* the type of an iterator, i.e. its functions */

/* experimental */
#undef JVEC_ITER_TYPE
#define JVEC_ITER_TYPE \
    jvec_iter (*add_int)  (jvec_iter, jlong);     /* add an integer to an iterator */ \
    int       (*cmp)      (jvec_iter, jvec_iter); /* compare two iterators */ \
    jvec_iter (*dec)      (jvec_iter);            /* decrement an iterator */ \
    T*        (*get)      (jvec_iter);            /* get the data pointed to by the iterator */ \
    jvec_iter (*inc)      (jvec_iter);            /* increment an interator */ \
    jvec_iter (*sub_int)  (jvec_iter, jlong);     /* subtract an integer from an interator */ \
    jlong     (*sub_iter) (jvec_iter, jvec_iter); /* subtract an iterator from another */ \

struct jvec_iter {
    JVEC_ITER_TYPE
    T* p;              /* the value of the iterator, a pointer to an element */
};

/* experimental */
#undef JVEC_TYPE
#define JVEC_TYPE \
    jvec_iter (*beginiter) (jvec*); /* get the start of the vector */ \
    jlong     (*capacity)  (jvec*); /* get the allocaed end of the vector */ \
    jvec_iter (*enditer)   (jvec*); /* get the logical end+1 of the vector */ \
    jerr      (*pop_back)  (jvec*); /* remove the last element of the vector */ \
    jerr      (*push_back) (jvec*, T*, jlong); /* add elements to the end of the vector, growing it if needed */ \
    jerr      (*resize)    (jvec*, jlong); /* change the logical size of the vector, smaller, larger, or unchanged */ \
    jlong     (*size)      (jvec*); /* get the logical size of the vector */ \

struct jvec {
    JVEC_TYPE
    jtype* telem; /* the type of the vector elements */
    T* begin;     /* the start of the vector */
    T* end;       /* the logical end+1 of the vector */
    T* cap;       /* the allocated end+1 of the vector */
};

void jvec_init(jvec* v);

#undef T
#undef jvec
#undef jvec_type
#undef jvec_iter
#undef jvec_iter_type
#undef jvec_init

#endif
