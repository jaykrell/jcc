/* jvec is like std::vector */
#ifndef T

#include "jvec.h"
#include "jerr.h"
#include "jmax.h"
#include <stdlib.h>
#include "jpaste.h"
#include "jssize.h"

#define T char
#include "jvec.c"

#define T int
#include "jvec.c"

#define T double
#include "jvec.c"

#else

#define jvec           JPASTE(jvec_, T)
#define jvec_type      JPASTE3(jvec_,T,_type)
#define jvec_iter      JPASTE3(jvec_,T,_iter)
#define jvec_iter_type JPASTE3(jvec_,T,_iter_type)
#define jvec_init      JPASTE3(jvec_,T,_init)

/* jvec_iter */
jvec_iter JPASTE(jvec_iter_type, _add_int)(jvec_iter ai, jssize_t i);
int JPASTE(jvec_iter_type, _cmp)(jvec_iter ai, jvec_iter aj);
jvec_iter JPASTE(jvec_iter_type, _dec)(jvec_iter ai);
T *JPASTE(jvec_iter_type, _get)(jvec_iter ai);
jvec_iter JPASTE(jvec_iter_type, _inc)(jvec_iter ai);
jvec_iter JPASTE(jvec_iter_type, _sub_int)(jvec_iter ai, jssize_t i);
jssize_t JPASTE(jvec_iter_type, _sub_iter)(jvec_iter ai, jvec_iter aj);

/* jvec */
jvec_iter JPASTE(jvec, _begin)(jvec *);
jssize_t JPASTE(jvec, _capacity)(jvec *);
jvec_iter JPASTE(jvec, _end)(jvec *);
jerr JPASTE(jvec, _pop_back)(jvec *);
jerr JPASTE(jvec, _push_back)(jvec *, T *, jssize_t);
jerr JPASTE(jvec, _resize)(jvec *, jssize_t);
jssize_t JPASTE(jvec, _size)(jvec *);

jvec_iter JPASTE(jvec_iter_type_, T) = {
    JPASTE(jvec_iter_type, _add_int),  JPASTE(jvec_iter_type, _cmp),
    JPASTE(jvec_iter_type, _dec),      JPASTE(jvec_iter_type, _get),
    JPASTE(jvec_iter_type, _inc),      JPASTE(jvec_iter_type, _sub_int),
    JPASTE(jvec_iter_type, _sub_iter),
};

jvec JPASTE(jvec_type_, T) = {
    JPASTE(jvec, _begin),    JPASTE(jvec, _capacity),  JPASTE(jvec, _end),
    JPASTE(jvec, _pop_back), JPASTE(jvec, _push_back), JPASTE(jvec, _resize),
    JPASTE(jvec, _size),

};

int JPASTE(jvec_iter_type, _cmp)(jvec_iter ai, jvec_iter aj) {
  T *p1 = ai.p;
  T *p2 = aj.p;
  return (p1 < p2) ? -1 : (p1 > p2) ? 1 : 0;
}

jvec_iter JPASTE(jvec_iter_type, _inc)(jvec_iter ai) {
  ++ai.p;
  return ai;
}

jvec_iter JPASTE(jvec_iter_type, _dec)(jvec_iter ai) {
  --ai.p;
  return ai;
}

jvec_iter JPASTE(jvec_iter_type, _add_int)(jvec_iter ai, jssize_t i) {
  ai.p += i;
  return ai;
}

jvec_iter JPASTE(jvec_iter_type, _sub_int)(jvec_iter ai, jssize_t i) {
  ai.p -= i;
  return ai;
}

jssize_t JPASTE(jvec_iter_type, _sub_iter)(jvec_iter ai, jvec_iter aj) {
  return (ai.p - aj.p);
}

T *JPASTE(jvec_iter_type, _get)(jvec_iter ai) { return ai.p; }

jvec_iter JPASTE(jvec, _begin)(jvec *v) {
  jvec_iter i = JPASTE(jvec_iter_type_, T);
  i.p = v->begin;
  return i;
}

jvec_iter JPASTE(jvec, _end)(jvec *v) {
  jvec_iter i = JPASTE(jvec_iter_type_, T);
  i.p = v->end;
  return i;
}

jssize_t JPASTE(jvec, _size)(jvec *v) { return (v->end - v->begin); }

jssize_t JPASTE(jvec, _capacity)(jvec *v) { return (v->cap - v->begin); }

jerr JPASTE(jvec, _push_back)(jvec *v, T *e, jssize_t n) {
  jerr err = {0};
  jssize_t size = {0};
  jtype *telem = {0};

  telem = v->telem;
  size = v->size(v);
  err = v->resize(v, size + n);
  if (err < 0)
    return err;
  T *new_elem = v->begin + size;
  if (telem)
    return telem->copy_to(e, new_elem);
  memmove(new_elem, e, sizeof(T) * n);
  return 0;
}

jerr JPASTE(jvec, _pop_back)(jvec *v) {
  jssize_t size = {0};

  size = v->size(v);
  if (size == 0)
    return 0;
  v->resize(v, size - 1);
  return 0;
}

jerr JPASTE(jvec, _resize)(jvec *v, jssize_t new_size) {
  jtype *telem = {0};
  jssize_t size = {0};
  T *begin = {0};
  jssize_t cap = {0};
  jssize_t new_cap = {0};

  telem = v->telem;
  size = v->size(v);
  begin = v->begin;
  if (size == new_size)
    return 0;
  if (new_size < size) {
    jssize_t to_cleanup = (size - new_size);
    if (telem)
      telem->cleanup(v->end - to_cleanup, to_cleanup);
    v->end -= to_cleanup;
    return 0;
  }
  cap = v->capacity(v);
  if (cap < new_size) {
    new_cap = JMAX(new_size, cap * 2);
    if (!(begin = (T *)calloc(new_cap, sizeof(T))))
      return jerr_out_of_memory;
    if (v->begin) {
      if (telem)
        telem->move(v->begin, begin, size);
      else
        memmove(begin, v->begin, size * sizeof(T));
      free(v->begin);
    }
    v->begin = begin;
    v->cap = begin + new_cap;
  }
  v->end = begin + new_size;
  if (telem)
    telem->init(begin + size, new_size - size);
  return 0;
}

void jvec_init(jvec *v) { *v = JPASTE(jvec_type_, T); }

#undef T
#undef jvec
#undef jvec_type
#undef jvec_iter
#undef jvec_iter_type
#undef jvec_init

#endif
