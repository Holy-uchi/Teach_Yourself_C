// 第２問
#include <stdio.h>

void print(int x, int y) { printf("x=%i, y=%i\n", x, y); }

int call_other(int (*fun)(int, int), void *arg1, void *arg2) {
  return fun(*(int *)arg1, *(int *)arg2);
}
