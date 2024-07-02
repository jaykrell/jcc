#include "jbase.h"
#include "jerr.h"
#include "jtype.h"

#ifndef T

#ifndef JVEC_H
#define JVEC_H

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

struct jvec;           typedef struct jvec           jvec;
struct jvec_type;      typedef struct jvec_type      jvec_type;
struct jvec_iter;      typedef struct jvec_iter      jvec_iter;
struct jvec_iter_type; typedef struct jvec_iter_type jvec_iter_type;

struct jvec_iter_type {
    jvec_iter (*add_int)  (jvec_iter*, jlong);
    int       (*cmp)      (jvec_iter*, jvec_iter*);
    jvec_iter (*dec)      (jvec_iter*);
    T*        (*get)      (jvec_iter*);
    jvec_iter (*inc)      (jvec_iter*);
    jvec_iter (*sub_int)  (jvec_iter*, jlong);
    jlong     (*sub_iter) (jvec_iter*, jvec_iter*);
};

struct jvec_iter {
    jvec_iter_type* t;
    T* p;
};

struct jvec_type {
    jvec_iter (*begin)     (jvec*);
    jlong     (*capacity)  (jvec*);
    jvec_iter (*end)       (jvec*);
    jerr      (*pop_back)  (jvec*);
    jerr      (*push_back) (jvec*, T*);
    jerr      (*resize)    (jvec*, jlong);
    jlong     (*size)      (jvec*);
};

struct jvec {
    jvec_type* t;
    jtype* telem;
    T* begin;
    T* end;
    T* cap;
};

#undef T
#undef jvec
#undef jvec_type
#undef jvec_iter
#undef jvec_iter_type
#undef jvec_init

#endif
