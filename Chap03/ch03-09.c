// #include <stdio.h>
// #include <limits.h>

// int main(void)
// {
//   long long value = LLONG_MAX;
//   printf("%lli\n", value);
//   return 0;
// }

// 第３問

// #include <stdio.h>
// #include <limits.h>

// int main(void)
// {
//   int imax;
//   unsigned int uimax;
//   long lmax;
//   unsigned long ulmax;
//   long long llmax;
//   unsigned long long ullmax;

//   imax = INT_MAX;
//   printf("%i\n", imax);

//   uimax = UINT_MAX;
//   printf("%u\n", uimax);

//   lmax = LONG_MAX;
//   printf("%li\n", lmax);

//   ulmax = ULONG_MAX;
//   printf("%lu\n", ulmax);

//   llmax = LLONG_MAX;
//   printf("%lli\n", llmax);

//   ullmax = ULLONG_MAX;
//   printf("%llu\n", ullmax);
//   return 0;
// }

// 第４問

#include <stdio.h>

int main(void) {
  printf("%lli, %llu\n", 32LL, 48ULL);
  return 0;
}
