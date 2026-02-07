// 第１問
// #include <stdio.h>
// #include <stdbool.h>
// #define RANGE_BEGIN 30
// #define RANGE_END 80

// int main(void)
// {
//   bool multiples[RANGE_END + 1] = {};

//   for (int i = 2; i <= RANGE_END; i++) {
//     if (multiples[i]) continue;

//     // 素数なら到達
//     if (i >= RANGE_BEGIN) {
//       printf("%i\n", i);
//     }

//     // 自分以上の数を倍数に設定
//     for (int j = 1; i * j <= RANGE_END; j++) {
//       multiples[i * j] = true;
//     }
//   }
// }

// 第２問 - 1
// #include <stdio.h>
// #include <stdint.h>

// int main(void)
// {
//   int16_t max_16 = 1;
//   int count_16 = 0;
//   for (int i = 1;; i++) {
//     if (i != 0 && max_16 > INT16_MAX / i) {
//       break;
//     }
//     max_16 *= i;
//     count_16 = i;
//   }
//   printf("16bit = %i, last_multiplier=%i\n", max_16, count_16);

//   int32_t max_32 = 1;
//   int count_32 = 0;
//   for (int32_t i = 1;; i++) {
//     if (i != 0 && max_32 > INT32_MAX / i) {
//       break;
//     }
//     max_32 *= i;
//     count_32 = i;
//   }
//   printf("32bit = %i, last_multiplier=%i\n", max_32, count_32);
// }

// 第２問 - 2
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  int16_t max_16 = 1;
  bool found_max_16 = false;
  int32_t max_32 = 1;
  bool found_max_32 = false;

  for (int i = 2;; i++) {
    if (!found_max_16 && max_16 <= INT16_MAX / i) {
      max_16 *= i;
      if (max_16 > INT16_MAX / i) {
        // 次で溢れるので終わり
        printf("16bit = %i, last multiplier=%i\n", max_16, i);
        found_max_16 = true;
      }
    }

    if (!found_max_32 && max_32 <= INT32_MAX / i) {
      max_32 *= i;
      if (max_32 > INT32_MAX / i) {
        // 次で溢れるので終わり
        printf("32bit = %i, last multiplier=%i\n", max_32, i);
        found_max_32 = true;
      }
    }

    if (found_max_16 && found_max_32) {
      break;
    }
  }
}
