// 第４問
// a. 2
// b. 8
// c. 12345
// d. 12345
// e. 1
// f. 16
// g. 3

// 第５問

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int comp(const void *a, const void *b) {
  int diff = *(const int *)a - *(const int *)b;
  return diff;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  size_t n = argc - 1;
  int base[n];
  for (size_t i = 0; i < n; i++) {
    base[i] = atoi(argv[i + 1]);
  }

  qsort(base, n, sizeof(base[0]), comp);

  for (size_t i = 0; i < n; i++) {
    printf("%i\n", base[i]);
  }
}
