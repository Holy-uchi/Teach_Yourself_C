// 第１問
// #include <stdio.h>
// #define ONE_TWO_THREE "123"
// #define FOUR_FIVE_SIX 456

// int main(void)
// {
//   puts(ONE_TWO_THREE);
//   printf("%i\n", FOUR_FIVE_SIX);
// }

// 第２問
// #include <stdio.h>
// #include <limits.h>
// #include <float.h>

// int main(void)
// {
//   int imax = INT_MAX;
//   int imin = INT_MIN; 
//   printf("int max: %i\n", imax);
//   printf("int min: %i\n", imin);

//   double dbl_max = DBL_MAX;
//   double dbl_min = -DBL_MAX;
//   printf("double max: %f\n", dbl_max);
//   printf("double min: %f\n", dbl_min);
// }

// 第３問
// #include <stdio.h>
// #include <limits.h>

// int main(void)
// {
//   printf("%llu\n", ULLONG_MAX);
//   printf("%lli\n", LLONG_MIN);
// }

// 第４問
// #include <stdio.h>
// #include <stdint.h>

// int main(void)
// {
//   int8_t a = -1;
//   int16_t b = a;
//   int32_t c = b;
//   int64_t d = c;
//   printf("a: %i, b: %i, c: %i, d: %lli\n", a, b, c, d);
// }

// 第５問
#include <stdio.h>
#include <stdint.h>

int main(void)
{
  uint8_t ui8 = -1;
  uint16_t ui16 = ui8;
  uint32_t ui32 = ui16;
  uint64_t ui64 = ui32;
  printf("ui8: %u, ui16: %u, ui32: %u, ui64: %llu\n", ui8, ui16, ui32, ui64);
}
