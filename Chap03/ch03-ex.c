// 第１問
// #include <stdio.h>
// #include <limits.h>

// int main(void)
// {
//   // long long x = INT_MAX;
//   // printf("x^2 %lli\n", x*x);
//   // return 0;

//   printf("%lli\n", (long long)INT_MAX * (long long)INT_MAX);
// }

// 第２問
// #include <stdio.h>
// #include <stdint.h>

// int main(void)
// {
//   // 2-1
//   // uint8_t x = 0xff;
//   // x--;
//   // printf("%i\n", (int8_t)x);

//   // 2-2
//   printf("%i\n", (int8_t)(0xffu - 1));
// }

// 第３問
// 10, 12, 12, 10, 10

// 第４問
#include <stdio.h>
#include <stdlib.h>
#define MUL(x, y) atoi(x) * (y)

int main(int argc, char *argv[]) {
  (void)argc;
  printf("%i\n", MUL(argv[1], atoi(argv[2])));
  printf("%i\n", MUL(argv[1], 8 - 3));
}
