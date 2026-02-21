#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_26.txt", "rb");
  if (!in_p) {
    perror("fopen");
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

  if (size == 0) {
    printf("empty\n");
    fclose(in_p);
    return 0;
  }

  // 非空なので最後のバイトを取る
  if (fseek(in_p, -1, SEEK_END) != 0) {
    perror("fseek");
    fclose(in_p);
    return 2;
  }
  int ch;
  if ((ch = fgetc(in_p)) == EOF) {
    if (ferror(in_p)) {
      perror("fgetc");
    } else {
      fprintf(stderr, "unexpected EOF\n");
    }
    fclose(in_p);
    return 2;
  }

  unsigned char uc = (unsigned char)ch;
  printf("%02X\n", uc);

  if (fclose(in_p)) {
    perror("fclose input");
    return 2;
  }

  return 0;
}
