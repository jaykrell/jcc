#ifndef T

#include "vec.h"

#define T int
#define JVec         JVecInt
#define JVecType     JVecIntType
#define JVecIter     JVecIntIter
#define JVecIterType JVecIntIterType
#define JVecInit     JVecIntInit
#include "vec.c"

#define T double
#define JVec         JVecDouble
#define JVecType     JVecDoubleType
#define JVecIter     JVecDoubleIter
#define JVecIterType JVecDoubleIterType
#define JVecInit     JVecDoubleInit
#include "vec.c"

#else

/* JVecIter */
int JPASTE (JVecIterType, _cmp)(JVecIter* ai, JVecIter* aj);
JVecIter JPASTE (JVecIterType, _inc)(JVecIter* ai);
JVecIter JPASTE (JVecIterType, _dec)(JVecIter* ai);
JVecIter JPASTE (JVecIterType, _add_int)(JVecIter* ai, Long i);
JVecIter JPASTE (JVecIterType, _sub_int)(JVecIter* ai, Long i);
Long JPASTE (JVecIterType, _sub_iter)(JVecIter* ai, JVecIter* aj);
T* JPASTE (JVecIterType, _get)(JVecIter* ai);

/* JVec */
JVecIter JPASTE (JVec, _begin)(JVec* a);
JVecIter JPASTE (JVec, _end)(JVec* a);
Long JPASTE (JVec, _push_back)(JVec* a, T* t);
jbool JPASTE (JVec, _pop_back)(JVec* a, T* t);

JVecIterType JPASTE (JVecIterType_, T) =
{
    JPASTE (JVecIterType, _cmp),
    JPASTE (JVecIterType, _inc),
    JPASTE (JVecIterType, _dec),
    JPASTE (JVecIterType, _add_int),
    JPASTE (JVecIterType, _sub_int),
    JPASTE (JVecIterType, _sub_iter),
    JPASTE (JVecIterType, _get),
};

JVecType JPASTE (JVecType_, T) =
{
    JPASTE (JVec, _begin),
    JPASTE (JVec, _end),
    JPASTE (JVec, _push_back),
    JPASTE (JVec, _pop_back),
};

int JPASTE (JVecIterType, _cmp)(JVecIter* ai, JVecIter* aj)
{
    T* p1 = ai->p;
    T* p2 = aj->p;
    return (p1 < p2) ? -1 : (p1 > p2) ? 1 :  0;
}

JVecIter JPASTE (JVecIterType, _inc)(JVecIter* ai)
{
    JVecIter i = {ai->t, ai->p + 1};
    return i;
}

JVecIter JPASTE (JVecIterType, _dec)(JVecIter* ai)
{
    JVecIter i = {ai->t, ai->p - 1};
    return i;
}

JVecIter JPASTE (JVecIterType, _add_int)(JVecIter* ai, Long i)
{
    JVecIter b = {ai->t, ai->p + i};
    return b;
}

JVecIter JPASTE (JVecIterType, _sub_int)(JVecIter* ai, Long i)
{
    JVecIter b = {ai->t, ai->p - i};
    return b;
}

Long JPASTE (JVecIterType, _sub_iter)(JVecIter* ai, JVecIter* aj)
{
    return (ai->p - aj->p);
}

T* JPASTE (JVecIterType, _get)(JVecIter* ai)
{
    return ai->p;
}

JVecIter JPASTE (JVec, _begin)(JVec* a)
{
    JVecIter i = {&JPASTE (JVecIterType_, T), a->begin};
    return i;
}

JVecIter JPASTE (JVec, _end)(JVec* a)
{
    JVecIter i = {&JPASTE (JVecIterType_, T), a->end};
    return i;
}

Long JPASTE (JVec, _push_back)(JVec* a, T* t)
{
    return 0;
}

jbool JPASTE (JVec, _pop_back)(JVec* a, T* t)
{
    return 0;
}

JErr JPASTE (JVec, _resize)(JVec* a, Long new_size)
{
    Long size = a->t->size(a);
    if (size == new_size)
        return 0;
    if (size < new_size) {
    }
    return 0;
}

void JVecInit(JVec* a)
{
    a->t = &JPASTE (JVecType_, T);
}

#undef T
#undef JVec
#undef JVecType
#undef JVecIter
#undef JVecIterType
#undef JVecInit

#endif
