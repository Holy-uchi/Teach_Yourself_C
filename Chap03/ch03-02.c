#include <stdio.h>
#include <stdlib.h>

//  int main(int argc, char *argv[])
//  {
//    int diff = atoi(argv[1]) - atoi(argv[2]);
//    printf("%d\n", diff);
//    return 0;
//  }

//  int main(int argc, char *argv[])
//  {
//    int product = atoi(argv[1]) * atoi(argv[2]);
//    printf("%d\n", product);
//    return 0;
//  }

int main(int argc, char *argv[])
{
  int quotient = atoi(argv[1]) / atoi(argv[2]);
  int remainder = atoi(argv[1]) % atoi(argv[2]);
  printf("%d...%d\n", quotient, remainder);
  return 0;
}