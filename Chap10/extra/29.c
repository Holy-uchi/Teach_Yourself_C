#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_29.txt", "rb");
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

  if (size == 0) {
    printf("\n");
    fclose(in_p);
    return 0;
  }
  for (long i = size; i-- > 0;) {
    if (fseek(in_p, i, SEEK_SET) != 0) {
      perror("fseek");
      fclose(in_p);
      return 2;
    }
    int ch = fgetc(in_p);
    if (ch == EOF) {
      if (ferror(in_p)) {
        perror("fgetc");
      } else {
        fprintf(stderr, "unexpected EOF at offset %ld\n", i);
      }
      fclose(in_p);
      return 2;
    }
    putchar(ch);
  }

  putchar('\n');

  if (fclose(in_p)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
