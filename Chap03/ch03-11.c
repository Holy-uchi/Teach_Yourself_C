// 第２問

#include <stdio.h>
#include <stdlib.h>
#define PRINT_MATH(operator) printf("%i\n", atoi(argv[1]) operator atoi(argv[2]))

int main(int argc, char *argv[])
{
  PRINT_MATH(+);
  PRINT_MATH(-);
  PRINT_MATH(*);
  PRINT_MATH(/);

  return 0;
}
