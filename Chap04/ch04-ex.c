// 第１問
// #include <stdio.h>
// #include <stdbool.h>
// #include <stdlib.h>

// bool odd(int n)
// {
//   return (n % 2 == 1) ? true : false;
// }

// int main(int argc, char *argv[])
// {
//   int x = atoi(argv[1]);
//   if (odd(x)) {
//     puts("odd");
//   } else {
//     puts("even");
//   }
// }

// 第２問
// #include <stdio.h>
// #include <stdlib.h>

// int main(int argc, char *argv[])
// {
//   if (argc != 2) {
//     puts("specify a number");
//     return 1;
//   }

//   int x = atoi(argv[1]);

//   if (x % 30 == 0) {
//     puts("C");
//   } else if (x % 10 == 0) {
//     puts("A");
//   } else if (x % 3 == 0) {
//     puts("B");
//   } else {
//     puts("D");
//   }
// }

// 第３問
// #include <stdio.h>
// #include <stdlib.h>

// int main(int argc, char *argv[])
// {
//   if (argc != 2) {
//     puts("specify a number");
//     return 1;
//   }

//   int x = atoi(argv[1]);
//   int remainder = x % 5;

//   switch (remainder) {
//     case 1:
//       puts("ONE");
//       break;
//     case 2:
//       puts("TWO");
//       break;
//     case 3:
//       puts("THREE");
//       break;
//     case 4:
//       puts("FOUR");
//       break;
//     case 0:
//       puts("ZERO");
//       break;
//   }
// }

// 第４問
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("specify a number");
    return 1;
  }

  int a = atoi(argv[1]);

  if (a <= 10) {
    puts("too small");
  } else if (a > 20) {
    puts("too large");
  } else if (a == 15) {
    puts("good");
  } else {
    puts("OK");
  }
}