// 第２問
// #include <stdio.h>

// int main(int argc, char *argv[])
// {
//   if (argc == 1) {
//     puts("Hello world!");
//   } else {
//     puts(argv[1]);
//   }
// }

// 第３問
#include <stdio.h>

int main(int argc, char *argv[]) {
  (void)argv;
  return (argc > 1) ? 0 : 1;
}
