#ifndef T

#include "jmax.h"
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
jvec_iter JPASTE (jvec_iter_type, _add_int)(jvec_iter* ai, jlong i);
int       JPASTE (jvec_iter_type, _cmp)(jvec_iter* ai, jvec_iter* aj);
jvec_iter JPASTE (jvec_iter_type, _dec)(jvec_iter* ai);
T*        JPASTE (jvec_iter_type, _get)(jvec_iter* ai);
jvec_iter JPASTE (jvec_iter_type, _inc)(jvec_iter* ai);
jvec_iter JPASTE (jvec_iter_type, _sub_int)(jvec_iter* ai, jlong i);
jlong     JPASTE (jvec_iter_type, _sub_iter)(jvec_iter* ai, jvec_iter* aj);

/* jvec */
jerr JPASTE (jvec, _begin)     (jvec*, jvec_iter*);
jerr JPASTE (jvec, _capacity)  (jvec*, jlong*);
jerr JPASTE (jvec, _end)       (jvec*, jvec_iter*);
jerr JPASTE (jvec, _pop_back)  (jvec*);
jerr JPASTE (jvec, _push_back) (jvec*, T*);
jerr JPASTE (jvec, _resize)    (jvec*, jlong);
jerr JPASTE (jvec, _size)      (jvec*, jlong*);

jvec_iter_type JPASTE (jvec_iter_type_, T) =
{
    JPASTE (jvec_iter_type, _add_int),
    JPASTE (jvec_iter_type, _cmp),
    JPASTE (jvec_iter_type, _dec),
    JPASTE (jvec_iter_type, _get),
    JPASTE (jvec_iter_type, _inc),
    JPASTE (jvec_iter_type, _sub_int),
    JPASTE (jvec_iter_type, _sub_iter),
};

jvec_type JPASTE (jvec_type_, T) =
{
    JPASTE (jvec, _begin),
    JPASTE (jvec, _capacity),
    JPASTE (jvec, _end),
    JPASTE (jvec, _pop_back),
    JPASTE (jvec, _push_back),
    JPASTE (jvec, _resize),
    JPASTE (jvec, _size),

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

jerr JPASTE (jvec, _begin)(jvec* v, jvec_iter* iter)
{
    jvec_iter i = {&JPASTE (jvec_iter_type_, T), v->begin};
    *iter = i;
    return 0;
}

jerr JPASTE (jvec, _end)(jvec* v, jvec_iter* iter)
{
    jvec_iter i = {&JPASTE (jvec_iter_type_, T), v->end};
    *iter = i;
    return 0;
}

jerr JPASTE (jvec, _size)(jvec* v, jlong* size)
{
    *size = (v->begin - v->end);
    return 0;
}

jerr JPASTE (jvec, _capacity)(jvec* v, jlong* cap)
{
    *cap = (v->cap - v->begin);
    return 0;
}

jerr JPASTE (jvec, _push_back)(jvec* v, T* e)
{
    jerr err;
    T* new_elem;
    jlong size;
    jvec_type* t;
    jtype* telem;

    t = v->t;
    telem = v->telem;
    err = t->size (v, &size);
    if (err < 0) return err;
    err = t->resize (v, size + 1);
    if (err < 0) return err;
    new_elem = v->begin + size;
    if (telem)
        return telem->copy_to (e, new_elem);
    memmove (new_elem, e, sizeof (T));
    return 0;
}

jerr JPASTE (jvec, _pop_back)(jvec* v)
{
    jerr err = {0};
    jvec_type* t = {0};
    jlong size = {0};

    t = v->t;
    err = t->size (v, &size);
    if (err < 0) return err;
    if (size == 0) return 0;
    t->resize (v, size - 1);
    return 0;
}

jerr JPASTE (jvec, _resize)(jvec* v, jlong new_size)
{
    jerr err = {0};
    jvec_type* t = {0};
    jtype* telem = {0};
    jlong size = {0};
    T* begin = {0};
    jlong cap = {0};
    jlong new_cap = {0};

    t = v->t;
    telem = v->telem;
    err = t->size (v, &size);
    if (err < 0) return err;
    if (size == new_size) return 0;
    if (new_size < size) {
        jlong to_cleanup = (size - new_size);
        if (telem)
            telem->cleanup (v->end - to_cleanup, to_cleanup);
        v->end -= to_cleanup;
        return 0;
    }
    err = t->capacity (v, &cap);
    if (err < 0) return err;
    if (cap < new_size) {
        new_cap = JMAX (new_size, cap * 2);
        if (!(begin = (T*)calloc (new_cap, sizeof (T))))
            return JErr_OutOfMemory;
        if (v->begin) {
            if (telem)
                telem->move (v->begin, begin, size);
            else
                memmove (begin, v->begin, size * sizeof (T));
            free (v->begin);
        }
        v->begin = begin;
        v->end = begin + size;
        v->cap = begin + new_cap;
        if (telem)
            telem->init (v->begin + size, new_size - size);
    }
    return 0;
}

void jvec_init(jvec* v)
{
    v->t = &JPASTE (jvec_type_, T);
}

#undef T
#undef jvec
#undef jvec_type
#undef jvec_iter
#undef jvec_iter_type
#undef jvec_init

#endif
