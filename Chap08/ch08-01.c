// 第１問
//  #include <stdint.h>
//
//  int main(void) {
//    // int16_t x;
//    // int16_t *y = &x;
//    // int32_t x;
//    // int32_t *y = &x;
//  }
//
//
//  第３問

#include <stdint.h>
#include <stdio.h>

int main(void) {
  int32_t x = 1234;
  int32_t *xp = &x;
  *xp += 4;
  printf("%i\n", *xp);
  printf("%p\n", (void *)xp);
}
