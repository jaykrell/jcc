#include "jbase.h"

#ifndef T

#ifndef JVEC_H
#define JVEC_H

#define T int
#define JVec         JVecInt
#define JVecType     JVecIntType
#define JVecIter     JVecIntIter
#define JVecIterType JVecIntIterType
#define JVecInit     JVecIntInit
#include "jarrayt.h"

#define T double
#define JVec         JVecDouble
#define JVecType     JVecDoubleType
#define JVecIter     JVecDoubleIter
#define JVecIterType JVecDoubleIterType
#define JVecInit     JVecDoubleInit
#include "jarrayt.h"

#endif

#else

struct JVec;         typedef struct JVec         JVec;
struct JVecType;     typedef struct JVecType     JVecType;
struct JVecIter;     typedef struct JVecIter     JVecIter;
struct JVecIterType; typedef struct JVecIterType JVecIterType;

struct JVecIterType {
    int        (*cmp)      (JVecIter*, JVecIter*);
    JVecIter (*inc)      (JVecIter*);
    JVecIter (*dec)      (JVecIter*);
    JVecIter (*add_int)  (JVecIter*, Long);
    JVecIter (*sub_int)  (JVecIter*, Long);
    Long       (*sub_iter) (JVecIter*, JVecIter*);
    T*         (*get)      (JVecIter*);
};

struct JVecIter {
    JVecIterType* t;
    T* p;
};

struct JVecType {
    JVecIter (*begin)     (JVec*);
    JVecIter (*end)       (JVec*);
    JErr     (*push_back) (JVec*, T*);
    jbool    (*pop_back)  (JVec*, T*);
    Long     (*size) (JVec*);
    JErr     (*resize) (JVec*);
};

struct JVec {
    JVecType* t;
    T* begin;
    T* end;
    T* cap;
};

#undef T
#undef JVec
#undef JVecType
#undef JVecIter
#undef JVecIterType
#undef JVecInit

#endif
