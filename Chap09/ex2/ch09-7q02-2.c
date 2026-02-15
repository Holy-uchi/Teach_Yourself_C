#include "ch09-7q02.h"
#include <stdio.h>

int add(int x, int y) { return x + y; }
int sub(int x, int y) { return x - y; }
int mul(int x, int y) { return x * y; }
int divide(int x, int y) { return x / y; }

CALC_FUNC select(char ch) {
  if (ch == '+') {
    return add;
  } else if (ch == '-') {
    return sub;
  } else if (ch == 'x' || ch == 'X') {
    return mul;
  } else if (ch == '/') {
    return divide;
  }

  else {
    return NULL;
  }
}
