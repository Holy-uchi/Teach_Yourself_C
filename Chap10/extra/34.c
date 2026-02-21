#include <stdio.h>

int main(void) {
  FILE *fp = fopen("data/Chap10/input_34.txt", "rb");
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

  rewind(fp);
  if (ferror(fp)) {
    perror("rewind");
    fclose(fp);
    return 2;
  }

  // 正しくないループ
  while (!feof(fp)) {
    int ch = fgetc(fp);
    printf("F:%d\n", ch);
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
