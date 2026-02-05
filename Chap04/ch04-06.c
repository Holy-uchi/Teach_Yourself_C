#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 4) {
    return 1;
  }

  int a = atoi(argv[2]);
  int b = atoi(argv[3]);

  switch (atoi(argv[1])) {
  case 0:
    printf("%i\n", a + b);
    break;
  case 1:
    printf("%i\n", a - b);
    break;
  case 2:
    printf("%i\n", a * b);
    break;
  case 3:
    printf("%i\n", a / b);
    break;
  default:
    puts("1st argument should be 0 to 3");
    return 2;
  }

  return 0;
}