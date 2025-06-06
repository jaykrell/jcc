/* test_vec.c */

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
#include "jvec1.h"
#include <assert.h>
#include <stdio.h>

/*************************************************************************************************/

int test_vec1(void) {
  jvec_double vd = {0};
  jvec_int vi = {0};
  jvec_int_iter vii = {0};
  int a = 1;
  double b = 1;
  long size = {0};
  int i = {0};

  /* test vector */

  jvec_int_init(&vi);
  jvec_double_init(&vd);

  size = (long)vi.size(&vi);
  printf("%ld\n", size);
  vi.push_back(&vi, &a, 1);
  size = (long)vi.size(&vi);
  printf("%ld\n", size);
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
  printf("cap: %ld\n", size = (long)vi.capacity(&vi));
  printf("size: %ld\n", size = (long)vi.size(&vi));

  for (i = 0; i < size; ++i)
    printf("%d ", vi.begin[i]);

  for (vii = vi.beginiter(&vi); vii.cmp(vii, vi.enditer(&vi));
       vii = vii.inc(vii))
    printf("%d ", *vii.get(vii));

  vd.push_back(&vd, &b, 1);
  size = (long)vd.size(&vd);
  for (i = 0; i < size; ++i)
    printf("%f ", vd.begin[i]);
  printf("\n");
  return 0;
}
