#include <stdio.h>

int main(void)
{
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 1;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    unsigned char uc = (unsigned char)ch;
    if (uc == 'a') {
      ch = '*';
    }

    if (fputc(ch, stdout) == EOF) {
      perror("fputc");
      fclose(fp);
      return 2;
    }
  }

  if (ferror(fp)) {
    perror("fgetc");
    fclose(fp);
    return 2;
  }

  if (fclose(fp) != 0) {
    perror("fclose");
    return 2;
  }
}
