#include <stdio.h>

int main(void) {
  int a, b, c;
  a = b = c = 0;

  int n = scanf("%i %i %i", &a, &b, &c);
  if (!n) {
    fputs("不正な入力です。\n", stderr);
  } else if (n == 3) {
    printf("%i\n", a + b + c);
  } else {
    fprintf(stderr, "%i個の合計は%iです。\n", n, a + b + c);
  }
}
