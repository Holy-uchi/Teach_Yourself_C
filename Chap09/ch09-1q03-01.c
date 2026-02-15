// CMAKE_SKIP_EXE
#include "ch09-1q03.h"
#include <stdio.h>

int ex1(int x, int y) {
  printf("ex1: %i\n", x + y);
  return x + y;
}

int main(void) { ex2(5, 6); }
