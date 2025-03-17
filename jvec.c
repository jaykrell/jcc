/* jcc, jlib, etc. by Jay Krell */
/* jvec is like std::vector */

#include "jvec.h"
#include "jmax.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int jvec_resize(jvec_generic *v, size_t new_size, size_t element_size) {
  size_t size = v->size;
  size_t capacity = v->capacity;

  if (new_size > capacity) {
    size_t new_capacity = JMAX(new_size, capacity * 2);
    char *new_data = (char *)realloc(v->data, new_capacity * element_size);
    if (!new_data)
      return -1;
    memset(new_data + size * element_size, 0, element_size);
    v->data = new_data;
    v->capacity = new_capacity;
  }
  v->size = new_size;
  return 0;
}

int jvec_insert(jvec_generic *v, void const *before, void const *begin,
                size_t count, size_t element_size) {
  size_t before_offset = ((char const *)before - v->data);
  size_t new_size = v->size + count;

  int err = jvec_resize(v, new_size, element_size);
  if (err)
    return err;
  memmove(v->data + before_offset + (count * element_size),
          v->data + before_offset, count * element_size);
  memmove(v->data + before_offset, begin, count * element_size);
  return 0;
}

int jvec_push_back(jvec_generic *v, void const *element, size_t element_size) {
  size_t size = v->size;
  int err = jvec_resize(v, size + 1, element_size);
  if (err)
    return err;

  memcpy(v->data + size * element_size, element, element_size);
  return 0;
}
