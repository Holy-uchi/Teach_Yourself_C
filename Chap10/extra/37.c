#include <stdio.h>

int main(void) {
  FILE *fp = fopen("data/Chap10/input_37.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  if (fputc('X', fp) == EOF) {
    perror("fputc X");
    fclose(fp);
    return 2;
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
