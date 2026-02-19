// 第３問

#include <stdio.h>

int main(void) {
  char buff[64];
  int a = 0;
  int b = 0;
  while (scanf("%*i,%63[^,],%i,%i", buff, &a, &b) == 3) {
    printf("%-11.11s %i-%i\n", buff, a, b);
  };
}
