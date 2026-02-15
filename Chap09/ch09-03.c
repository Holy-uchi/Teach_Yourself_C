//  第１問 - 1
//  #include <stdio.h>
//
//  void swap(int *x, int *y) {
//    int tmp = *x;
//    *x = *y;
//    *y = tmp;
//  }
//  int main(void) {
//    int x = 8;
//    int y = 18;
//    swap(&x, &y);
//    printf("%i, %i\n", x, y);
//  }
//
//  第１問 - 2
//  #include <stdio.h>
//  #include <stdlib.h>
//
//  int add(const char *x, const char *y) { return atoi(x) + atoi(y); }
//
//  // int main(int argc, char *argv[]) {
//  int main(int argc, char **argv) {
//    if (argc != 3) {
//      puts("usage: a.exe number number");
//      return 1;
//    }
//
//    printf("%i\n", add(argv[1], argv[2]));
//  }
//
//  第３問
//  #include <stdio.h>
//
//  int div(int x, int y, int *rem) {
//    *rem = x % y;
//    return x / y;
//  }
//
//  int main(void) {
//    int rem;
//    int q = div(13, 4, &rem);
//    printf("13 / 4 = %i ... %i\n", q, rem);
//  }

#include <stdarg.h>
#include <stdio.h>

void x(int *np, ...) {
  va_list ap;
  va_start(ap, np);
  for (int i = 0;; i++) {
    int *p = va_arg(ap, int *);
    if (!p) {
      break;
    }
    *p = i;
  }
  va_end(ap);
}

int main(void) {
  int a, b, c, d, e, f, count = 6;
  x(&count, &a, &b, &c, &d, &e, &f, NULL);
  printf("%i, %i, %i, %i, %i, %i\n", a, b, c, d, e, f);
}
