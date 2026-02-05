// 第１問
// #include <stdio.h>
// 
// int main(void)
// {
//   int x = 1;
//   for (;;) {
//     x += x * 2;
//     if (x < 0) {
//       break;
//     }
//   }
//   printf("%i\n", x);
// }

// 第２問
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int target = INT_MAX;
  for (int i = 1; i < argc; i++) {
    int divisor = atoi(argv[i]);
    if (divisor == 0) {
      puts("divisor is 0");
      return 1;
    }
    target /= divisor;
  }
  printf("%i\n", target);
}