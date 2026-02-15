#include <stdio.h>

int main(void)

{
  int p;
  for (int i = 0; i < 5; i++) {
    if (i == 3) {
      p = i;
    }
  }
  printf("%i\n", p);
}
