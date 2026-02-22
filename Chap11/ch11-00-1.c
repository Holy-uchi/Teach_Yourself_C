#include <stdio.h>

int main(void) {

  int a;
  double b;
  int c;
  char str[16];
  char d4[8];
  while (scanf(" %d %lf %x %15s", &a, &b, &c, str) == 4) {
    snprintf(d4, sizeof(d4), "%6.3f", b);
    d4[5] = '\0';
    printf("%06d,%s,0x%08x,%s!\n", a, d4, c, str);
  }

  return 0;
}
