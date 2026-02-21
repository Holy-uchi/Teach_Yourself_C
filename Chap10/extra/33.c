#include <stdbool.h>
#include <stdio.h>
#define SIZE_REQUIRED 10

bool filesize(FILE *fp, long *size);

int main(void) {
  FILE *fp = fopen("data/Chap10/input_33.txt", "rb+");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  long size = 0;
  if (!filesize(fp, &size)) {
    fclose(fp);
    return 2;
  }

  if (size < SIZE_REQUIRED) {
    fprintf(stderr, "File size too small: Got %li bytes; Required: %i\n", size, SIZE_REQUIRED);
    fclose(fp);
    return 2;
  }

  for (int i = 0; i < 3; i++) {
    int ch = fgetc(fp);
    if (ch == EOF) {
      if (ferror(fp)) {
        perror("fgetc");
      } else {
        fprintf(stderr, "unexpected EOF at offset %i\n", i);
      }
      fclose(fp);
      return 2;
    }
  }

  if (fseek(fp, 0, SEEK_CUR) != 0) {
    // 位置は変えないが、切り替えを成立させる。
    perror("fseek");
    fclose(fp);
    return 2;
  }
  int ch = fputc('X', fp);
  if (ch == EOF) {
    perror("fputc");
    fclose(fp);
    return 2;
  }

  if (fflush(fp) != 0) {
    perror("fflush");
    fclose(fp);
    return 2;
  }

  rewind(fp);

  char buff[256];
  while (fgets(buff, sizeof(buff), fp) != NULL) {
    printf("%s", buff);
  }

  if (ferror(fp)) {
    perror("fgets");
    fclose(fp);
    return 2;
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}

bool filesize(FILE *fp, long *size) {
  long cur = ftell(fp);
  if (cur == -1L) {
    perror("ftell");
    return false;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    perror("fseek");
    return false;
  }
  long size_local = ftell(fp);
  if (size_local == -1L) {
    perror("ftell");
    return false;
  }

  if (fseek(fp, cur, SEEK_SET) != 0) {
    perror("fseek");
    return false;
  }
  *size = size_local;
  return true;
}
