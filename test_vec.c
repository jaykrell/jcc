/* test_vec.c */

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
#include "jsize.h"
#include "jvec.h"
#include <assert.h>
#include <stdio.h>

/*************************************************************************************************/

int main(void) {
  JVEC(double) vd = {0};
  JVEC(int) vi = {0};
  int *vii = {0};
  int a = 1;
  double b = 1;
  size_t size = {0};
  size_t i = {0};

  /* test vector */

  size = vi.size;
  printf("%" JSIZE_PRI "d\n", size);
  JVEC_PUSH_BACK(&vi, &a);
  size = vi.size;
  printf("%" JSIZE_PRI "d\n", size);
  ++a;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  a *= 2;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  ++a;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  a *= 3;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  ++a;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  a *= 4;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  ++a;
  JVEC_PUSH_BACK(&vi, &a);
  JVEC_PUSH_BACK(&vi, &a);
  printf("cap: %" JSIZE_PRI "d\n", size = vi.capacity);
  printf("size: %" JSIZE_PRI "d\n", size = vi.size);

  for (i = 0; i < size; ++i)
    printf("%d ", vi.data[i]);

  for (vii = vi.data; vii != (vi.data + vi.size); ++vii)
    printf("%d ", *vii);

  JVEC_PUSH_BACK(&vd, &b);
  size = vd.size;
  for (i = 0; i < size; ++i)
    printf("%f ", vd.data[i]);

  printf("\n");
  return 0;
}
