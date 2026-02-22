#include <stdio.h>

int main(void) {
  FILE *fp = fopen("data/Chap10/input_36.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (putchar(ch) == EOF) {
      perror("putchar");
      fclose(fp);
      return 2;
    }
  }

  if (ferror(fp)) {
    perror("fgetc");
    fclose(fp);
    return 2;
  }

  printf("before clear: feof = %i\n", feof(fp));
  clearerr(fp);
  printf("after clear: feof = %i\n", feof(fp));

  rewind(fp);

  while ((ch = fgetc(fp)) != EOF) {
    if (putchar(ch) == EOF) {
      perror("putchar");
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

  return 0;
}
