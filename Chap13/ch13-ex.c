// 第１問
// #include <stdio.h>
// #include <stdlib.h>
//
// int main(int argc, char *argv[]) {
//   if (argc < 2) {
//     return 1;
//   }
//
//   int num = atoi(argv[1]);
//
//   int num_inversed = ~num;
//   printf("%i\n", num_inversed + 1);
// }

// 第２問

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if (argc != 3) {
    return 1;
  }

  int base = atoi(argv[1]);
  int base_abs = base < 0 ? (~base + 1) : base;
  int devider = atoi(argv[2]);
  int devider_abs = devider < 0 ? (~devider + 1) : devider;
  int quot_abs = 0;
  bool is_quot_minus = (bool)(base < 0) ^ (bool)(devider < 0);

  int bit = 0;
  while (base_abs >= (devider_abs << (bit + 1))) {
    bit++;
  }

  int remainder = base_abs;
  while (bit >= 0) {
    if (remainder >= (devider_abs << bit)) {
      remainder -= (devider_abs << bit);
      quot_abs |= 1 << bit;
    }
    bit--;
  }
  int quot = is_quot_minus ? (~quot_abs + 1) : quot_abs;
  remainder = base < 0 ? (~remainder + 1) : remainder;
  printf("%i / %i = %i ... %i\n", base, devider, quot, remainder);
}
