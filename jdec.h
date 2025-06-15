/* jcc, jlib, etc. by Jay Krell */
/* jdec is like std::vector of std::deque
   It is a vector with an offset. */
#if !JDEC_H
#define JDEC_H 1

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
#define JDEC(T)                                                                \
  struct {                                                                     \
    T *data;                                                                   \
    ptrdiff_t size;                                                            \
    ptrdiff_t capacity;                                                        \
    int static_;                                                               \
    T *prealloc;                 /*TODO*/                                      \
    ptrdiff_t prealloc_capacity; /*TODO*/                                      \
    struct {                                                                   \
      T *base;                                                                 \
      T *end;                                                                  \
    } internal;                                                                \
  }
/*TODO: size_t? */

typedef JDEC(char) jdec_generic;

void jdec_cleanup(jdec_generic *);
int jdec_push_back(jdec_generic *, void const *element, ptrdiff_t element_size);
int jdec_resize(jdec_generic *, ptrdiff_t new_size, ptrdiff_t element_size);
int jdec_reserve(jdec_generic *, ptrdiff_t new_size, ptrdiff_t element_size);
int jdec_insert(jdec_generic *, void const *before, void const *begin,
                ptrdiff_t count, ptrdiff_t element_size);

#define JDEC_CLEANUP(v) jdec_cleanup((jdec_generic *)(v))

#define JDEC_RESIZE(v, size)                                                   \
  jdec_resize((jdec_generic *)(v), size, sizeof((v)->data[0]))

#define JDEC_END(v) ((v)->data + (v)->size)
#define JDEC_PUSH_BACK(v, element)                                             \
  jdec_push_back((jdec_generic *)(v), element, sizeof((v)->data[0]))

#define JDEC_INSERT(v, before, begin, count)                                   \
  jdec_insert((jdec_generic *)(v), (before), (begin), (count),                 \
              sizeof((v)->data[0]))
#define JDEC_APPEND(v, begin, count) JDEC_INSERT(v, JDEC_END(v), begin, count)
#define JDEC_RESERVE(v, n)                                                     \
  jdec_reserve((jdec_generic *)(v), (n), sizeof((v)->data[0]))

typedef JDEC(char) jdec_char_t;
typedef JDEC(uint64_t) jdec_uint64_t;

#if __cplusplus
}
#endif
#endif
