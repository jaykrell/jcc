#ifndef T

#include "jarrayt.h"

#define T int
#define JArray         JArrayInt
#define JArrayType     JArrayIntType
#define JArrayIter     JArrayIntIter
#define JArrayIterType JArrayIntIterType
#include "jarrayt.c"

#define T double
#define JArray         JArrayDouble
#define JArrayType     JArrayDoubleType
#define JArrayIter     JArrayDoubleIter
#define JArrayIterType JArrayDoubleIterType
//#include "jarrayt.c"

#else

int JPASTE (JArrayIterType, _cmp)(JArrayIter* ai, JArrayIter* aj);
JArrayIter JPASTE (JArrayIterType, _inc)(JArrayIter* ai);
JArrayIter JPASTE (JArrayIterType, _dec)(JArrayIter* ai);
JArrayIter JPASTE (JArrayIterType, _add_int)(JArrayIter* ai, Long i);
JArrayIter JPASTE (JArrayIterType, _sub_int)(JArrayIter* ai, Long i);
JArrayIter JPASTE (JArrayIterType, _sub_iter)(JArrayIter* ai, JArrayIter* aj);
T* JPASTE (JArrayIterType, _get)(JArrayIter* ai);

JArrayIter JPASTE (JArray, _begin)(JArray* a);
JArrayIter JPASTE (JArray, _end)(JArray* a);
Long JPASTE (JArray, _push_back)(JArray* a, T* t);
jbool JPASTE (JArray, _pop_back)(JArray* a, T* t);

JArrayIterType JPASTE (JArrayIterType_, T) =
{
    JPASTE (JArrayIterType, _cmp),
    JPASTE (JArrayIterType, _inc),
    JPASTE (JArrayIterType, _dec),
    JPASTE (JArrayIterType, _add_int),
    JPASTE (JArrayIterType, _sub_int),
    JPASTE (JArrayIterType, _sub_iter),
    JPASTE (JArrayIterType, _get),
};

JArrayType JPASTE (JArrayType_, T) =
{
    JPASTE (JArray, _begin),
    JPASTE (JArray, _end),
    JPASTE (JArray, _push_back),
    JPASTE (JArray, _pop_back),
};

int JPASTE (JArrayIterType, _cmp)(JArrayIter* ai, JArrayIter* aj)
{
    T* p1 = ai->p;
    T* p2 = aj->p;
    return (p1 < p2) ? -1 : (p1 > p2) ? 1 :  0;
}

JArrayIter JPASTE (JArrayIterType, _inc)(JArrayIter* ai)
{
    JArrayIter i = {ai->t, ai->p + 1};
    return i;
}

JArrayIter JPASTE (JArrayIterType, _dec)(JArrayIter* ai)
{
    JArrayIter i = {ai->t, ai->p - 1};
    return i;
}

JArrayIter JPASTE (JArrayIterType, _add_int)(JArrayIter* ai, Long i)
{
    JArrayIter b = {ai->t, ai->p + i};
    return b;
}

JArrayIter JPASTE (JArrayIterType, _sub_int)(JArrayIter* ai, Long i)
{
    JArrayIter b = {ai->t, ai->p - i};
    return b;
}

JArrayIter JPASTE (JArrayIterType, _sub_iter)(JArrayIter* ai, JArrayIter* aj)
{
    JArrayIter b = {ai->t, ai->p - aj->p};
    return b;
}

T* JPASTE (JArrayIterType, _get)(JArrayIter* ai)
{
    return ai->p;
}

JArrayIter JPASTE (JArray, _begin)(JArray* a)
{
    JArrayIter i = {&JPASTE (JArrayIterType_, T), a->begin};
    return i;
}

JArrayIter JPASTE (JArray, _end)(JArray* a)
{
    JArrayIter i = {&JPASTE (JArrayIterType_, T), a->end};
    return i;
}

Long JPASTE (JArray, _push_back)(JArray* a, T* t)
{
    return 0;
}

jbool JPASTE (JArray, _pop_back)(JArray* a, T* t)
{
    return 0;
}

#if 0

void jarray_free(voidp av)
{
    jarray_t* a = (jarray_t*)av;
    jarray_iter iter = {0};
    jarray_iter end = {0};

    a->t->begin(&iter);
    a->t->end(&end);

    while (!iter.eq(&end))
    {
        jobject_t* data = {0};

        iter.get(&data);
        data->t->free(data);
        iter.inc(&iter);
    }
}

Long jarray_dup(voidp, voidp*)
{
}

Long jarray_dupto(voidp, voidp)
{
}

Long jarray_valid(voidp)
{
}

void jarray_begin(voidp, voidp*);

void jarray_end(voidp av, voidp* iter)
{
}


void jarray_inc(voidp pv)
{
}

Long jarray_size(voidp pv)
{
}

void jarray_dump(voidp pv)
{
}


jarrayt Jarrayt =
{
    "array",
    0, /* fixed */
    0, /* fixed */
};

int jarray_init(jarray_t* ary)
{

}

#endif

#undef T
#undef JArray
#undef JArrayType
#undef JArrayIter
#undef JArrayIterType

#endif
