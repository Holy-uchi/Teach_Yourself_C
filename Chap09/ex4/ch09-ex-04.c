
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int64_t power1(int64_t current, int n) {
  if (n == 0) {
    return 1;
  }
  return current * power1(current, n - 1);
}

int64_t power2(int64_t root, int n) {
  int64_t result = 1;
  for (int i = 1; i <= n; i++) {
    result *= root;
  }

  return result;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }

  int64_t res1 = power1(6, atoi(argv[1]));
  double perc1 = 1 / (double)res1;

  int64_t res2 = power2(6, atoi(argv[1]));
  double perc2 = 1 / (double)res2;
  printf("1/%lli(%f)\n", res1, perc1);
  printf("1/%lli(%f)\n", res2, perc2);

  return 0;
}
