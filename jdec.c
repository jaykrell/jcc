/* jcc, jlib, etc. by Jay Krell */
/* jdec is like std::vector of std::deque
   It is a vector with an offset. */
#include "jdec.h"
#include "jmax.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void jdec_cleanup(jdec_generic *v) {
  free(v->data);
  /*JMEMSET0_POINTER(v);*/
  v->data = 0;
}

int jdec_reserve(jdec_generic *v, ptrdiff_t new_capacity,
                 ptrdiff_t element_size) {
  ptrdiff_t capacity = v->capacity;
  if (new_capacity > capacity) {
    char *new_data = (char *)realloc(v->data, new_capacity * element_size);
    if (!new_data)
      return -1;
    memset(new_data + v->size * element_size, 0, element_size);
    v->data = new_data;
    v->capacity = new_capacity;
  }
  return 0;
}

int jdec_resize(jdec_generic *v, ptrdiff_t new_size, ptrdiff_t element_size) {
  int err;
  ptrdiff_t capacity = v->capacity;

  if (new_size > capacity) {
    ptrdiff_t new_capacity = JMAX(new_size, capacity * 2);
    if ((err = jdec_reserve(v, new_capacity, element_size)))
      return err;
  }
  v->size = new_size;
  return 0;
}

int jdec_insert(jdec_generic *v, void const *before, void const *begin,
                ptrdiff_t count, ptrdiff_t element_size) {
  ptrdiff_t before_offset = ((char const *)before - v->data);
  ptrdiff_t new_size = v->size + count;

  int err = jdec_resize(v, new_size, element_size);
  if (err)
    return err;
  memmove(v->data + before_offset + (count * element_size),
          v->data + before_offset, count * element_size);
  memmove(v->data + before_offset, begin, count * element_size);
  return 0;
}

int jdec_push_back(jdec_generic *v, void const *element,
                   ptrdiff_t element_size) {
  ptrdiff_t size = v->size;
  int err = jdec_resize(v, size + 1, element_size);
  if (err)
    return err;

  memcpy(v->data + size * element_size, element, element_size);
  return 0;
}
