// 第１問
// #include <stdio.h>
//
// int main(int argc, char *argv[])
// {
//   for (int i = 1; i < argc; i++) {
//     puts(argv[i]);
//   }
// }

// 第２問
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  (void)argc;

  int last = atoi(argv[1]);
  int sum = 0;
  for (int i = 1; i <= last; i++) {
    sum += i;
  }
  printf("%i\n", sum);
}
