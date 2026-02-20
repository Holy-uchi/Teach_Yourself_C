// 第６問
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void)
{
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    fprintf(stderr, "%s", strerror(errno));
    return 2;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    unsigned char uc = (unsigned char)ch;
    if (isalpha(uc)) {
      ch = toupper(uc);
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

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }
}
