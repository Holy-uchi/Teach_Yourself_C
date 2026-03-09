// 第１問

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// bool is_odd(int num) { return num & 1; }
//
// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     return 1;
//   }
//   int num = atoi(argv[1]);
//   if (is_odd(num)) {
//     puts("!");
//   } else {
//     puts("?");
//   }
// }

// 第２問
int main(int argc, char *argv[]) {
  if (argc < 2) {
    return 1;
  }

  unsigned char uc = argv[1][0];
  if (uc < 65 || uc > 90) {
    puts("not a capital letter!");
    return 1;
  }

  uc |= 0x20;
  printf("%c\n", uc);
}
