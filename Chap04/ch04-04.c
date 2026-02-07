// 第１問
// #include <stdio.h>

// int main(int argc, char *argv[])
// {
//   if (argc == 3) {
//     puts("Bingo!");
//   } else {
//     puts("Oops!");
//   }
// }

// 第２問
// #include <stdio.h>
// #include <stdlib.h>

// int main(int argc, char *argv[])
// {
//   if (argc != 2) {
//     puts("specify a number");
//     return 0;
//   }

//   int num = atoi(argv[1]); // argv[1]が数字じゃない時は知らね
//   if (num == 123) {
//     puts("Bingo!");
//   } else if ( num > 123) {
//     puts("greater");
//   } else {
//     puts("less");
//   }
// }

// 第３問
// #include <stdio.h>

// int main(int argc, char *argv[])
// {
//   if (argc == 2) {
//     puts("argc == 2");
//   } else if (argc > 5) {
//     puts("argc > 5");
//   } else if (argc > 1) {
//     puts("argc > 1");
//   } else {
//     puts("not match");
//   }
// }

// 第４問
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    puts("no arguments");
    return 1;
  }

  int num = atoi(argv[1]);

  if (num % 15 == 0) {
    puts("FizBaz");
  } else if (num % 3 == 0) {
    puts("Fiz");
  } else if (num % 5 == 0) {
    puts("Baz");
  } else {
    printf("%i\n", num);
  }
}
