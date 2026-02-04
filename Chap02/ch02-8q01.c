#include <stdio.h>
#include <stdlib.h>

// 第一問

//  int main(int argc, char *argv[])
//  {
//    // puts("Hello!");
//       puts("Good morning!");
//      /*
//        puts("Good afternoon!");
//       */
//       puts("Good night!");
//  }

// 第二問
//  int main(void)
//  {
//    puts("Hello world!");
//  }

// 第三問
int sum(int x, int y)
{
  return x + y;
}


int main(int arvc, char *argv[])
{
  return sum(atoi(argv[1]), atoi(argv[2]));
}