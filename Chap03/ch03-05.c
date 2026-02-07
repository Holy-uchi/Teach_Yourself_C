#include <stdio.h>

int main(int argc, char *argv[]) {
  int x = 1;
  x = x + 1;

  x += 1;

  ++x;

  x++;

  printf("%d\n", x);
  return 0;
}
