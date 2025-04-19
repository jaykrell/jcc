/* jvec is like std::vector
 * jvec1 is earlier version, probably not to use. */
#if !JVEC_H
#define JVEC_H 1

#include "jssize.h"
#include <stdint.h>

#if __cplusplus
extern "C" {
#endif

/* In order to be idiomatic to user, size and capacity are in elements, not
 * bytes users rarely/never use capacity, but they will use and this is
 * consistent between them. We may come to regret this, i.e. avoid
 * multiplication or division in the library and stick division by constant in
 * user. */
#define JVEC(T)                                                                \
  struct {                                                                     \
    T *data;                                                                   \
    ptrdiff_t size;                                                            \
    ptrdiff_t capacity;                                                        \
  }

typedef JVEC(char) jvec_generic;

void jvec_cleanup(jvec_generic *);
int jvec_push_back(jvec_generic *, void const *element, ptrdiff_t element_size);
int jvec_resize(jvec_generic *, ptrdiff_t new_size, ptrdiff_t element_size);
int jvec_insert(jvec_generic *, void const *before, void const *begin,
                ptrdiff_t count, ptrdiff_t element_size);

#define JVEC_CLEANUP(v) jvec_cleanup((jvec_generic *)(v))

#define JVEC_RESIZE(v, size)                                                   \
  jvec_resize((jvec_generic *)(v), size, sizeof((v)->data[0]))

#define JVEC_END(v) ((v)->data + (v)->size)
#define JVEC_PUSH_BACK(v, element)                                             \
  jvec_push_back((jvec_generic *)(v), element, sizeof((v)->data[0]))

#define JVEC_INSERT(v, before, begin, count)                                   \
  jvec_insert((jvec_generic *)(v), (before), (begin), (count),                 \
              sizeof((v)->data[0]))
#define JVEC_APPEND(v, begin, count) JVEC_INSERT(v, JVEC_END(v), begin, count)
#define JVEC_RESERVE(v)

typedef JVEC(char) jvec_char_t;
typedef JVEC(uint64_t) jvec_uint64_t;

#if __cplusplus
}
#endif
#endif
