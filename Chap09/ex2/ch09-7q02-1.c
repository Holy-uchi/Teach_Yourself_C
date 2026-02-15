#include "ch09-7q02.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    puts("usage: a.exe +/-/x/X// number number");
    return 1;
  }

  int (*calc)(int, int) = select(argv[1][0]);
  if (calc) {
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    printf("%i %c %i = %i\n", x, argv[1][0], y, calc(x, y));
  } else {
    puts("wrong operator");
  }
}
