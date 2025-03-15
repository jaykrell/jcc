/* test_vec.c */

#pragma warning(disable : 4100) /* unused parameter */
#include "jvec1.h"
#include <assert.h>
#include <stdio.h>

/*************************************************************************************************/

int main(void) {
  jvec_double vd = {0};
  jvec_int vi = {0};
  jvec_int_iter vii = {0};
  int a = 1;
  double b = 1;
  size_t size = {0};
  size_t i = {0};

  /* test vector */

  jvec_int_init(&vi);
  jvec_double_init(&vd);

  size = vi.size(&vi);
  printf("%" JLONG_PRI "d\n", size);
  vi.push_back(&vi, &a, 1);
  size = vi.size(&vi);
  printf("%" JLONG_PRI "d\n", size);
  ++a;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  a *= 2;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  ++a;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  a *= 3;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  ++a;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  a *= 4;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  ++a;
  vi.push_back(&vi, &a, 1);
  vi.push_back(&vi, &a, 1);
  printf("cap: %" JLONG_PRI "d\n", size = vi.capacity(&vi));
  printf("size: %" JLONG_PRI "d\n", size = vi.size(&vi));

  for (i = 0; i < size; ++i)
    printf("%d ", vi.begin[i]);

  for (vii = vi.beginiter(&vi); vii.cmp(vii, vi.enditer(&vi)); vii = vii.inc(vii))
    printf("%d ", *vii.get(vii));

  vd.push_back(&vd, &b, 1);
  size = vd.size(&vd);
  for (i = 0; i < size; ++i)
    printf("%f ", vd.begin[i]);
}
