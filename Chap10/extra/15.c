#include <ctype.h>
#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/input.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  FILE *out_p = fopen("data/all_capital.txt", "wb");
  if (!out_p) {
    perror("fopen output");
    fclose(in_p);
    return 2;
  }

  int ch;
  while ((ch = fgetc(in_p)) != EOF) {
    unsigned char uc = (unsigned char)ch;
    if (isalpha(uc)) {
      ch = toupper(uc);
    }

    if (fputc(ch, out_p) == EOF) {
      perror("write output");
      fclose(in_p);
      fclose(out_p);
      return 2;
    }
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    fclose(out_p);
    return 2;
  }

  if (fclose(in_p)) {
    perror("fclose input");
  }

  if (fclose(out_p)) {
    perror("fclose output");
    fclose(in_p);
    return 2;
  }

  return 0;
}
