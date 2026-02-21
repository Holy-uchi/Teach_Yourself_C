#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_28.txt", "rb+");
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

  if (size < 10) {
    fprintf(stderr, "File size too small: %li\n", size);
    fclose(in_p);
    return 2;
  }

  if (fseek(in_p, 9, SEEK_SET) != 0) {
    perror("fseek");
    fclose(in_p);
    return 2;
  }

  if (fputc('X', in_p) == EOF) {
    perror("fputc");
    fclose(in_p);
    return 2;
  }

  if (fclose(in_p)) {
    perror("fclose input");
    return 2;
  }

  return 0;
}
