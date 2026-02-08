// 第１問

// b: 要素数3, 1,2,3
// c: 要素数3, 1,2,3
// d: 要素数4, すべて不定
// e: 要素数4, すべて0
// f: 要素数4, 1, 0, 0, 0

// 第２問

// #include <stdio.h>
//
// int main(void) {
//   int a[10];
//
//   for (int i = 0; i < 10; i++) {
//     if (i % 2 == 1) {
//       a[i] = i * 10;
//     } else {
//       a[i] = i * 2;
//     }
//   }
//
//   for (int j = 0; j < 10; j++) {
//     printf("%i\n", a[j]);
//   }
// }

// 第３問
// #include <_inttypes.h>
// #include <stdio.h>
//
// int main(void) {
//   int x[10];
//   for (int i = 0, j = 9; i < 10; i++, j--) {
//     x[i] = j;
//   }
//
//   for (int k = 0; k < 10; k++) {
//     printf("%i\n", x[k]);
//   }
// }

// 第４問
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int prod = 1;
  for (int i = 1; i < argc; i++) {
    int num = atoi(argv[i]);
    if (!num) {
      continue;
    }
    prod *= num;
  }
  printf("%i\n", prod);
}
