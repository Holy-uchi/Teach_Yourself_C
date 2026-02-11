// // 例文
//
// #include <stdio.h>
// int main(void) {
//   int ch = '\n';
//   do {
//     if (ch == '\n') {
//       puts("It's OK? (Y/N)");
//     }
//     ch = getchar();
//   } while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N');
//   if (ch == 'y' || ch == 'Y') {
//     puts("Yes!");
//   } else {
//     puts("No!");
//   }
// }

// 第２問

#include <stdio.h>
int main(void) {
  // for文を使う場合
  for (int n = 1; n <= 10; n++) {
    printf("%i\n", n);
  }

  // while文を使う場合
  int n = 1;
  while (n <= 10) {
    printf("%i\n", n);
    n++;
  }

  // do-while文を使う場合
  // nを使いまわす
  n = 1;
  do {
    printf("%i\n", n);
  } while (++n <= 10);
}
