#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  (void)argc;
  int x = atoi(argv[1]);
  int y = atoi(argv[2]);
  int sum, diff, prod, quot;
  sum = x + y;
  diff = x - y;
  prod = x * y;
  quot = x / y;
  printf("%d, %d, %d, %d\n", sum, diff, prod, quot);
  return 0;
}
