/* jcc, jlib, etc. by Jay Krell */
/* jdec is like std::vector of std::deque
   It is a vector with an offset. */
#include "jdec.h"
#include "jmax.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

ptrdiff_t jdec_capacity(jdec_generic *v) {
  assert(v->internal.end >= v->internal.base);
  return v->internal.end - v->internal.base;
}

void jdec_assert(jdec_generic *v) {
  ptrdiff_t capacity=0;

  capacity = jdec_capacity(v);

  assert(v->internal.base <= v->data);
  assert((v->data + v->size) <= v->internal.end);
  assert(v->size >= 0);
  assert(capacity >= v->size);
}

void jdec_cleanup(jdec_generic *v) {
  free(v->data);
  /*JMEMSET0_POINTER(v);*/
  v->data = 0;
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
  ptrdiff_t size=0;

  capacity = jdec_capacity(v);
  capacity = (capacity ? capacity : element_size);
  capacity *= 2;
  assert((capacity % element_size) == 0);

  new_base = (char *)malloc(capacity);
  if (!new_base)
    return -1;

  size = v->size;
  before = ((capacity - size) / 2);

  memcpy(new_base + before, v->data, size);
  free(v->internal.base);
  v->data = new_base + before;
  v->internal.base = new_base;
  v->internal.end = (new_base + capacity);
  /* size is unchanged here, caller typically changes it */
  return 0;
}

int jdec_push_front(jdec_generic *v, void const *element,
                    ptrdiff_t element_size) {
  int err;
  if (!v->data || (v->data - element_size) < v->internal.base) {
    err = jdec_internal_grow(v, element_size);
    if (err)
      return err;
  }

  memcpy(v->data -= element_size, element, element_size);
  v->size += element_size;
  jdec_assert(v);
  return 0;
}

int jdec_push_back(jdec_generic *v, void const *element,
                   ptrdiff_t element_size) {
  ptrdiff_t size=0;
  int err=0;

  jdec_assert(v);
  size = v->size;

  if (!v->data || (v->data + v->size) >= v->internal.end) {
    err = jdec_internal_grow(v, element_size);
    if (err)
      return err;
  }

  memcpy(v->data + size * element_size, element, element_size);
  v->size = (size + element_size);
  jdec_assert(v);
  return 0;
}
