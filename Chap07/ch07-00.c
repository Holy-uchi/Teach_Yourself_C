// 第１問

//  #include <stdio.h>
//  #define BEGIN 0
//  #define LAST 10
//  int main(void) {
//    int n = BEGIN;
//    while (++n <= LAST) {
//      printf("%i\n", n);
//    }
//  }

// 第２問
//
//  #include <stdio.h>
//  #define BEGIN 0
//  #define END 10
//
//  int main(void) {
//    int n = BEGIN;
//    do {
//      printf("%i\n", n);
//    } while (++n < END);
//  }

// 第３問

#include <stdio.h>

int main(void) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-braces"
  int a[][3] = {
      1, 2, 3, 4, 5, 6, 7, 8, 9,
  };
#pragma clang diagnostic pop
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      int v0 = a[i][j];
      printf("%i\n", v0);
    }
  }
}
