// 第３問-1

//  #include <stdint.h>
//  #include <stdio.h>
//
//  int main(void) {
//    uint32_t x = 1;
//    uint32_t *x_ptr = &x;
//    if (*(uint8_t *)x_ptr == 1) {
//      puts("little endian");
//    } else {
//      puts("big endian");
//    }
//  }

// 第３問 - ２
//  #include <stddef.h>
//  #include <stdint.h>
//  #include <stdio.h>
//  #include <stdlib.h>
//
//  int main(int argc, char *argv[]) {
//    if (argc != 2) {
//      return 1;
//    }
//
//    uint32_t x = atoi(argv[1]);
//    uint32_t *x_ptr = &x;
//    uint32_t y;
//    uint32_t *y_ptr = &y;
//    size_t bytes = sizeof(uint32_t) / sizeof(uint8_t);
//    for (size_t i = 0; i < bytes; i++) {
//      ((uint8_t *)y_ptr)[i] = ((uint8_t *)x_ptr)[bytes - 1 - i];
//    }
//    printf("%08x\n", y);
//  }

// 第４問
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  (void)argc;
  size_t len = strlen(argv[1]);
  for (size_t i = len; i-- > 0;) {
    printf("%c", argv[1][i]);
  }
  printf("\n");
}
