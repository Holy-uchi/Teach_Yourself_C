// CMAKE_SKIP_EXE
#include "ch09-7q01.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }
  for (int i = 1; i < argc; i++) {
    int n = atoi(argv[i]);
    if (n % 2 == 1) {
      odd(i, n);
    } else {
      even(i, n);
    }
  }
}
