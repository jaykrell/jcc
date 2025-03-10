#include "jarray.h"

void jarray_free(jvoidp av) {
  jarray_t *a = (jarray_t *)av;
  jarray_iter iter = {0};
  jarray_iter end = {0};

  a->t->begin(&iter);
  a->t->end(&end);

  while (!iter.eq(&end)) {
    jobject_t *data = {0};

    iter.get(&data);
    data->t->free(data);
    iter.inc(&iter);
  }
}

jlong jarray_dup(jvoidp, jvoidp *) {}

jlong jarray_dupto(jvoidp, jvoidp) {}

jlong jarray_valid(jvoidp) {}

void jarray_begin(jvoidp, jvoidp *);

void jarray_end(jvoidp av, jvoidp *iter) {}

void jarray_inc(jvoidp pv) {}

jlong jarray_size(jvoidp pv) {}

void jarray_dump(jvoidp pv) {}

jarrayt Jarrayt = {
    "array", 0, /* fixed */
    0,          /* fixed */
};

int jarray_init(jarray_t *ary) {}
