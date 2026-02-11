// 第１問
// #include <_stdio.h>
// #include <stdio.h>
// int main(void) {
//   // for
//   for (int n = 9; n >= 0; n--) {
//     printf("%i\n", n);
//   }
//
//   int n = 9;
//   while (n >= 0) {
//     printf("%i\n", n);
//     n--;
//   }
//
//   n = 9;
//   do {
//     printf("%i\n", n);
//   } while (--n >= 0);
// }

// 第２問

// #include <stdio.h>
//
// int main(void) {
//   int count = 0;
//   char ch = 0;
//   do {
//     ch = fgetc(stdin);
//     printf("%c", ch);
//     if (ch == '\n') {
//       count++;
//     }
//   } while (count < 3);
//   return 0;
// }

//  #include <stdio.h>
//
//  int main(void) {
//    int a[2][3];
//    a[0][0] = 1;
//    a[0][1] = 2;
//    a[0][2] = 3;
//    a[1][0] = 4;
//    a[1][1] = 5;
//    a[1][2] = 6;
//
//    int i = 0;
//    do {
//      int j = 0;
//      while (j < 3) {
//        printf("%i\n", a[i][j]);
//        j++;
//      }
//    } while (++i < 2);
//  }

#include <stdio.h>

int main(void) {
  int a[2][3] = {{1, 2, 3}, {4, 5, 6}};

  int i = 0;
  do {
    int j = 0;
    while (j < 3) {
      printf("%i\n", a[i][j]);
      j++;
    }
  } while (++i < 2);
}
