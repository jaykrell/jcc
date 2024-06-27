#ifndef T

#include "jvec.h"

#define T int
#define jvec           jvec_int
#define jvec_type      jvec_int_type
#define jvec_iter      jvec_int_iter
#define jvec_iter_type jvec_int_iter_type
#define jvec_init      jvec_int_init
#include "jvec.c"

#define T double
#define jvec           jvec_double
#define jvec_type      jvec_double_type
#define jvec_iter      jvec_double_iter
#define jvec_iter_type jvec_double_iter_type
#define jvec_init      jvec_double_init
#include "jvec.c"

#else

/* jvec_iter */
int JPASTE (jvec_iter_type, _cmp)(jvec_iter* ai, jvec_iter* aj);
jvec_iter JPASTE (jvec_iter_type, _inc)(jvec_iter* ai);
jvec_iter JPASTE (jvec_iter_type, _dec)(jvec_iter* ai);
jvec_iter JPASTE (jvec_iter_type, _add_int)(jvec_iter* ai, jlong i);
jvec_iter JPASTE (jvec_iter_type, _sub_int)(jvec_iter* ai, jlong i);
jlong JPASTE (jvec_iter_type, _sub_iter)(jvec_iter* ai, jvec_iter* aj);
T* JPASTE (jvec_iter_type, _get)(jvec_iter* ai);

/* jvec */
jvec_iter JPASTE (jvec, _begin)(jvec* a);
jvec_iter JPASTE (jvec, _end)(jvec* a);
jlong JPASTE (jvec, _push_back)(jvec* a, T* t);
jbool JPASTE (jvec, _pop_back)(jvec* a, T* t);

jvec_iter_type JPASTE (jvec_iter_type_, T) =
{
    JPASTE (jvec_iter_type, _cmp),
    JPASTE (jvec_iter_type, _inc),
    JPASTE (jvec_iter_type, _dec),
    JPASTE (jvec_iter_type, _add_int),
    JPASTE (jvec_iter_type, _sub_int),
    JPASTE (jvec_iter_type, _sub_iter),
    JPASTE (jvec_iter_type, _get),
};

jvec_type JPASTE (jvec_type_, T) =
{
    JPASTE (jvec, _begin),
    JPASTE (jvec, _end),
    JPASTE (jvec, _push_back),
    JPASTE (jvec, _pop_back),
};

int JPASTE (jvec_iter_type, _cmp)(jvec_iter* ai, jvec_iter* aj)
{
    T* p1 = ai->p;
    T* p2 = aj->p;
    return (p1 < p2) ? -1 : (p1 > p2) ? 1 :  0;
}

jvec_iter JPASTE (jvec_iter_type, _inc)(jvec_iter* ai)
{
    jvec_iter i = {ai->t, ai->p + 1};
    return i;
}

jvec_iter JPASTE (jvec_iter_type, _dec)(jvec_iter* ai)
{
    jvec_iter i = {ai->t, ai->p - 1};
    return i;
}

jvec_iter JPASTE (jvec_iter_type, _add_int)(jvec_iter* ai, jlong i)
{
    jvec_iter b = {ai->t, ai->p + i};
    return b;
}

jvec_iter JPASTE (jvec_iter_type, _sub_int)(jvec_iter* ai, jlong i)
{
    jvec_iter b = {ai->t, ai->p - i};
    return b;
}

jlong JPASTE (jvec_iter_type, _sub_iter)(jvec_iter* ai, jvec_iter* aj)
{
    return (ai->p - aj->p);
}

T* JPASTE (jvec_iter_type, _get)(jvec_iter* ai)
{
    return ai->p;
}

jvec_iter JPASTE (jvec, _begin)(jvec* a)
{
    jvec_iter i = {&JPASTE (jvec_iter_type_, T), a->begin};
    return i;
}

jvec_iter JPASTE (jvec, _end)(jvec* a)
{
    jvec_iter i = {&JPASTE (jvec_iter_type_, T), a->end};
    return i;
}

jlong JPASTE (jvec, _push_back)(jvec* a, T* t)
{
    return 0;
}

jbool JPASTE (jvec, _pop_back)(jvec* a, T* t)
{
    return 0;
}

jerr JPASTE (jvec, _resize)(jvec* a, jlong new_size)
{
    T* begin = {0};
    jlong cap = {0};
    jlong size = {0};
    jtype* t = {0};
    jtype* telem = {0};

    t = a->t;
    telem = a->telem;
    size = t->size (a);
    if (size == new_size)
        return 0;
    if (size < new_size) {
        jlong to_cleanup = (new_size - size);
        if (telem)
            telem->cleanup (a->end - to_cleanup, to_cleanup);
        a->end -= to_cleanup;
        return 0;
    }
    cap = a->t->capacity (a);
    if (cap < new_size) {
        new_cap = JMAX (new_size, cap * 2);
        begin = (T*)calloc (new_cap, sizeof (T));
        if (a->begin) {
            if (telem)
                telem->move (a->begin, begin, size);
            else
                memmove (begin, a->begin, size * sizeof (T));
            free (a->begin);
        }
        a->begin = begin;
        a->end = begin + size;
        a->cap = begin + new_cap;
        if (telem)
            telem->init (a->begin + size, new_size - size);
    }
    return 0;
}

void jvec_init(jvec* a)
{
    a->t = &JPASTE (jvec_type_, T);
}

#undef T
#undef jvec
#undef jvec_type
#undef jvec_iter
#undef jvec_iter_type
#undef jvec_init

#endif
