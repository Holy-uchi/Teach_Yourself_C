// 第１問
// #include <stdio.h>

// int main(void)
// {
//   double d = 321.253;
//   printf("%i\n", (int)d);

//   int i = d;
//   printf("%i\n", i);
//   return 0;
// }

// 第２問
#include <limits.h>
#include <stdio.h>

int main(void) {
  long long x = (unsigned int)INT_MIN;
  printf("%lli\n", x);
}
