#include <stdint.h>
#include <stdio.h>

int main(void) {
  int32_t array[] = {
      1,
      2,
      3,
      4,
  };
  unsigned char u = 'x';
  int32_t *p0 = array;
  int32_t *p3 = &array[2];
  unsigned char *up = &u;
  printf("%i, %i, %c\n", *p0, *p3, *up);
}
