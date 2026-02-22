#include <stdio.h>

int main(void) {
  FILE *fp = fopen("data/Chap10/input_35.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  printf("1回目\n");

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

  printf("\n\n");
  rewind(fp);

  printf("2回目\n");

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
    perror("fp");
    return 2;
  }

  return 0;
}
