//  // 第１問
//  #include <stdint.h>
//
//  int32_t (*add_p)(int32_t x, int32_t y, int32_t z);
//  double (*add2_p)(double x, double y);
//  char *(*select_p)(char *a, char *b, char *c);
//  int (*main_p)(int argc, char *argv[]);
//  void (*change_p)(char c, char *cp, int[]);
//  int (*run_p)(int (*fun1)(int, int), int (*fun2)(int, int));
//
//  // 第２問
//  void afunc(void);
//  int32_t bfunc(int32_t[]);
//  char *cfunc(const char *p0, const char *p1);

// 第３問
#include <complex.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comp(const void *a, const void *b) {
  const char *const *pa = a;
  const char *const *pb = b;
  return strcmp(*pa, *pb);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return 1;
  }
  int n = argc - 1;
  char *base[n];
  for (int i = 0; i < n; i++) {
    base[i] = argv[i + 1];
  }
  qsort(&base, n, sizeof(base[0]), comp);

  for (int i = 0; i < n; i++) {
    puts(base[i]);
  }
}
