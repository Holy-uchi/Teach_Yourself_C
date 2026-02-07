#include <stdint.h>
#include <stdio.h>

int main(void) {
  // 第１問
  // int16_t x = -10;
  // int32_t y = x;
  // printf("x: %d, y: %d\n", x, y);
  // return 0;

  // 第２問
  // uint16_t x = -10;
  // uint32_t y = x;
  // printf("x: %d, y: %d\n", x, y);
  // return 0;

  // 第３問
  // int32_t x = -10;
  // uint16_t y = x;
  // printf("x: %d, y: %d\n", x, y);
  // return 0;

  // 第４問
  int32_t x = -10;
  int16_t y = x;
  uint16_t z = y;
  x = z;
  printf("x: %d, y: %d\n", x, y);
  return 0;
}
