// 第１問
// #include <stdio.h>
// #include <stdbool.h>

// int main(void)
// {
//   bool found = false;
//   for (int x = 0; x < 5; x++) {
//     for (int y = 0; y < 5; y++) {
//       for (int z = 0; z < 5; z++) {
//         printf("x:%i, y:%i, z:%i\n", x, y, z);
//         if (x == 1 && y == 2 && z == 3) {
//           found = true;
//           break;
//         }
//       }
//       if (found) {
//         break;
//       }
//     }
//     if (found) {
//       break;
//     }
//   }
// }

// 第２問
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  bool found = false;
  for (int i = 0; i < 8; i++) {
    switch (i % 3) {
    case 0:
      puts("Fizz");
      break;
    case 1:
      puts("Fizz + 1");
      break;
    case 2:
      found = true;
      break;
    default:
      puts("bug! never come here");
    }

    if (found)
      break;
  }
  puts("end");
}