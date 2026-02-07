// 第１問
// #include <stdio.h>
// #include <stdlib.h>

// int main(int argc, char *argv[])
// {
//   double x = atof(argv[1]);
//   double y = atof(argv[2]);

//   double sum;
//   double diff;
//   double prod;
//   double quot;

//   sum = x + y;
//   diff = x - y;
//   prod = x * y;
//   quot = x / y;

//   printf("sum: %f, diff: %f, prod: %f, quot: %f\n", sum, diff, prod, quot);
//   return 0;
// }

// 第２問
#include <float.h>
#include <stdio.h>

int main(void) {
  long double ldbl_max = LDBL_MAX;
  printf("%Lf\n", ldbl_max);
  return 0;
}