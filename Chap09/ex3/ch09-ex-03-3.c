#include <stdio.h>
#include <stdlib.h>

int add(int x) {
  static int total = 0;
  total += x;
  return total;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }
  for (int i = 1; i < argc; i++) {
    printf("%i\n", add(atoi(argv[i])));
  }
}
