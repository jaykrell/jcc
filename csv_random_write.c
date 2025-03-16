/* write a random file */
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
int main() {
	int i, j, imax, jmax;

  srand((int)time(0));

  imax = rand() % 0x1000;

  for (i = 0; i < imax; ++i) {
    jmax = rand() % 0x1000;
    for (int j = 0; j < jmax; ++j) {
      printf("%d", (int)rand());
      if (j + 1 != jmax)
        printf(",");
    }
    printf("\n");
  }
}
