// 第２問

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int32_t change_endian(int32_t x) {
  int32_t result = 0;
  for (int i = 0; i < 4; i++) {
    int32_t mask = 0xff << (8 * i); // 下から2桁ずつ舐めていく
    int32_t byte = (x & mask) >> (8 * i);
    result += byte << (8 * (3 - i));
  }
  return result;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }

  int32_t num = atoi(argv[1]);
  int32_t transformed = change_endian(num);

  printf("%#.8x => %#.8x\n", num, transformed);
}
