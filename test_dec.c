/* test_dec.c */

#if _MSC_VER
#pragma warning(disable : 4100) /* unused parameter */
#endif
#include "jdec.h"
#include "jint64.h"
#include "jsize.h"
#include <assert.h>
#include <stdio.h>

/*************************************************************************************************/

int test_dec(void) {
  JDEC(double) vd = {0};
  JDEC(int) vi = {0};
  int *vii = 0;
  int a = 1;
  double b = 1;
  jsize size = 0;
  jsize i = 0;

  /* test vector */

  size = vi.size;
  printf("%" JSIZE_PRI "d\n", size);
  JDEC_PUSH_BACK(&vi, &a);
  size = vi.size;
  printf("%" JSIZE_PRI "d\n", size);
  ++a;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  a *= 2;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  ++a;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  a *= 3;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  ++a;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  a *= 4;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  ++a;
  JDEC_PUSH_BACK(&vi, &a);
  JDEC_PUSH_BACK(&vi, &a);
  /*printf("cap: %" JSIZE_PRI "d\n", size = vi.capacity);*/
  printf("size: %" JSIZE_PRI "d\n", size = vi.size);

  for (i = 0; i < size; ++i)
    printf("%d ", vi.data[i]);

  for (vii = vi.data; vii != (vi.data + vi.size); ++vii)
    printf("%d ", *vii);

  printf("\n-- float --\n");
  JDEC_PUSH_BACK(&vd, &b);
  size = vd.size;
  for (i = 0; i < size; ++i)
    printf("%f ", vd.data[i]);

  printf("\n-- push_front --\n");
  ++a;
  JDEC_PUSH_FRONT(&vi, &a);
  JDEC_PUSH_FRONT(&vi, &a);
  printf("size: %" JSIZE_PRI "d\n", size = vi.size);
  for (vii = vi.data; vii != (vi.data + vi.size); ++vii)
    printf("%d ", *vii);

  printf("\n");
  return 0;
}
