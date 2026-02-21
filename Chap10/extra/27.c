#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_27.txt", "rb");
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

  if (size < 100) {
    fprintf(stderr, "file size too small: %li\n", size);
    fclose(in_p);
    return 2;
  }

  // 1-based の 「100バイト目」= 0-based offset 99
  if (fseek(in_p, 99, SEEK_SET) != 0) {
    perror("fseek");
    fclose(in_p);
    return 2;
  }

  int ch;
  if ((ch = fgetc(in_p)) == EOF) {
    if (ferror(in_p)) {
      perror("fgetc");
    } else {
      fprintf(stderr, "unexpected EOF at %li\n", size);
    }
    fclose(in_p);
    return 2;
  }

  unsigned char uc = (unsigned char)ch;
  printf("%02X\n", uc);

  if (fclose(in_p)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
