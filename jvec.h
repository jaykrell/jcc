/* jvec is like std::vector */

#ifndef JVEC_H
#define JVEC_H 1

#include <stddef.h>

#if __cplusplus
extern "C" {
#endif

#define JVEC(T)                                                                \
  struct {                                                                     \
    T *data;                                                                   \
    size_t size, capacity;                                                     \
  }

typedef JVEC(char) jvec_generic;

int jvec_push_back(jvec_generic *, void *element, size_t element_size);
int jvec_resize(jvec_generic *, size_t count, size_t element_size);
int jvec_insert(jvec_generic *, void *before, void *begin, size_t count,
                size_t element_size);

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

#if __cplusplus
}
#endif
#endif
