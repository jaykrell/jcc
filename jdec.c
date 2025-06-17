/* jcc, jlib, etc. by Jay Krell */
/* jdec is like std::vector of std::deque
   It is a vector with an offset. */
#include "jdec.h"
#include "jmax.h"
#include "jmem.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

ptrdiff_t jdec_capacity(jdec_generic *v) {
  assert(v->internal.end >= v->internal.base);
  return v->internal.end - v->internal.base;
}

void jdec_assert(jdec_generic *v) {
  assert(v->internal.base <= v->data);
  assert((v->data + v->size) <= v->internal.end);
  assert(v->size >= 0);
  assert(jdec_capacity(v) >= v->size);
}

void jdec_cleanup(jdec_generic *v) {
  free(v->internal.base);
  JMEMSET0_POINTER(v);
}

int jdec_internal_grow(jdec_generic *v, ptrdiff_t element_size) {
  /* Policy:
   - double (zero goes 2)
   - divide extra space evenly, even though
   usage patterns might favor otherwise
   */
  char *new_base = 0;
  ptrdiff_t before = 0;
  ptrdiff_t capacity = 0;

  jdec_assert(v);
  capacity = jdec_capacity(v);
  capacity = (capacity ? capacity : 1);
  capacity *= 2;

  new_base = (char *)malloc(capacity * element_size);
  if (!new_base)
    return -1;

  before = ((capacity - v->size) / 2 * element_size);

  memcpy(new_base + before, v->data, v->size * element_size);
  free(v->internal.base);
  v->data = (new_base + before);
  v->internal.base = new_base;
  v->internal.end = (new_base + capacity * element_size);
  jdec_assert(v);
  return 0;
}

int jdec_push_front(jdec_generic *v, void const *element,
                    ptrdiff_t element_size) {
  int err = 0;
  jdec_assert(v);
  if ((v->data - element_size) < v->internal.base) {
    err = jdec_internal_grow(v, element_size);
    if (err)
      return err;
  }

  memcpy(v->data -= element_size, element, element_size);
  v->size += 1;
  jdec_assert(v);
  return 0;
}

int jdec_push_back(jdec_generic *v, void const *element,
                   ptrdiff_t element_size) {
  int err = 0;
  jdec_assert(v);
  if ((v->data + (v->size * element_size)) >= v->internal.end) {
    err = jdec_internal_grow(v, element_size);
    if (err)
      return err;
  }

  memcpy(v->data + v->size * element_size, element, element_size);
  v->size += 1;
  jdec_assert(v);
  return 0;
}
