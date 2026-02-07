// 第１問
// #include <stdio.h>

// int main(void)
// {
//   for (int i = 1; i <= 30; i++) {
//     if (i % 6 == 0) continue;
//     printf("%i\n", i);
//   }
// }

// 第２問
#include <stdio.h>

int main(void) {
  for (int i = 1; i <= 50; i++) {
    if (i % 15 == 0) {
      puts("FizzBuzz");
      continue;
    } else if (i % 3 == 0) {
      puts("Fizz");
      continue;
    } else if (i % 5 == 0) {
      puts("Buzz");
      continue;
    }

    printf("%i\n", i);
  }
}
