#include "jbase.h"

#ifndef T

#ifndef JARRAYT_H
#define JARRAYT_H

#define T int
#define JArray         JArrayInt
#define JArrayType     JArrayIntType
#define JArrayIter     JArrayIntIter
#define JArrayIterType JArrayIntIterType
#define JArrayInit     JArrayIntInit
#include "jarrayt.h"

#define T double
#define JArray         JArrayDouble
#define JArrayType     JArrayDoubleType
#define JArrayIter     JArrayDoubleIter
#define JArrayIterType JArrayDoubleIterType
#define JArrayInit     JArrayDoubleInit
#include "jarrayt.h"

#endif

#else

struct JArray;         typedef struct JArray         JArray;
struct JArrayType;     typedef struct JArrayType     JArrayType;
struct JArrayIter;     typedef struct JArrayIter     JArrayIter;
struct JArrayIterType; typedef struct JArrayIterType JArrayIterType;

struct JArrayIterType {
    int        (*cmp)      (JArrayIter*, JArrayIter*);
    JArrayIter (*inc)      (JArrayIter*);
    JArrayIter (*dec)      (JArrayIter*);
    JArrayIter (*add_int)  (JArrayIter*, Long);
    JArrayIter (*sub_int)  (JArrayIter*, Long);
    Long       (*sub_iter) (JArrayIter*, JArrayIter*);
    T*         (*get)      (JArrayIter*);
};

struct JArrayIter {
    JArrayIterType* t;
    T* p;
};

struct JArrayType {
    JArrayIter (*begin)     (JArray*);
    JArrayIter (*end)       (JArray*);
    Long       (*push_back) (JArray*, T*);
    jbool      (*pop_back)  (JArray*, T*);
};

struct JArray {
    JArrayType* t;
    T* begin;
    T* end;
    T* cap;
};

void JArrayInit (JArray*);

#undef T
#undef JArray
#undef JArrayType
#undef JArrayIter
#undef JArrayIterType
#undef JArrayInit

#endif
