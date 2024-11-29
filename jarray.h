#ifndef JARRAY_H
#define JARRAY_H

#include "jbase.h"
#include "jtype.h"

typedef struct jarray_iter jarray_iter;
typedef struct jarrayt jarrayt;
typedef struct jarray_t jarray_t;

struct jarray_iter {
    jlong pos;
    int (*eq)(jarray_iter*);
    void (*inc)(jarray_iter*);
    voidp (*get)(jarray_iter*);
};

struct jarrayt {
    jtype t;
    void (*begin)(jarray_iter*);
    void (*end)(jarray_iter*);
    void (*push_back)(voidp);
    void (*pop_back)(voidp);
};

struct jarray_t {
    jarrayt* t;
    jtype* telem;
    charp begin, end, cap;
    jlong size;
    jlong capacity;
};

int jarray_init(jarray_t*);

#endif
