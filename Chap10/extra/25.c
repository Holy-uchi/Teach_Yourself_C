#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_25.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  if (fseek(in_p, 0, SEEK_END) != 0) {
    perror("fseek");
    fclose(in_p);
    return 2;
  }

  long size = ftell(in_p);
  if (size == -1L) {
    perror("ftell");
    fclose(in_p);
    return 2;
  }

  rewind(in_p);

  printf("size: %li\n", size);

  if (fclose(in_p)) {
    perror("fclose input");
    return 2;
  }

  return 0;
}
