/* genprimes.c */

#pragma warning(disable : 4100) /* unused parameter */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
/*************************************************************************************************/
int main(void) {
  const unsigned N = 1000;
  const unsigned M = 1000;
  char *vec = (char *)calloc(M, N);
  int64_t i = {0};
  int64_t j = {0};
  for (i = 3; i < N; i += 2) {
    j = i;
    while (1) {
      j += i;
      if (j >= (M * N))
        break;
      vec[j] = 1;
    }
  }
  j = 0;
  int64_t prev = 5;
  for (i = 3; i < N * M; i += 2) {
    if (!vec[i]) {
      if (i > (prev * 3)) {
        prev = i;
        printf("%" JLONG_PRI "d, ", i);
        if (++j == 8) {
          printf("\n");
          j = 0;
        }
      }
    }
  }
}
