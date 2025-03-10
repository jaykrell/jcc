/* write a random file */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
  srand(static_cast<int>(time(0)));

  int imax = rand() % 0x1000;

  for (int i = 0; i < imax; ++i) {
    int jmax = rand() % 0x1000;
    for (int j = 0; j < jmax; ++j) {
      printf("%d", (int)rand());
      if (j + 1 != jmax)
        printf(",");
    }
    printf("\n");
  }
}
