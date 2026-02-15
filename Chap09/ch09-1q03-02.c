#include "ch09-1q03.h"
#include <stdio.h>

int ex2(int x, int y) {
  printf("ex2: %i\n", x * y);
  return ex1(x, y);
}
