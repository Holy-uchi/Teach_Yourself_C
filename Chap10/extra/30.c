#include <stdbool.h>
#include <stdio.h>

bool filesize(FILE *fp, long *size);

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_30.txt", "rb+");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  long size = 0;
  if (!filesize(in_p, &size)) {
    fclose(in_p);
    return 2;
  }
  printf("initial file size: %li\n", size);

  if (size < 3) {
    fprintf(stderr, "File size too small: %libytes\n", size);
    fclose(in_p);
    return 2;
  }

  rewind(in_p);
  for (int i = 0; i < 3; i++) {
    int ch = fputc('X', in_p);
    if (ch == EOF) {
      perror("fputc");
      fclose(in_p);
      return 2;
    }
  }

  if (!filesize(in_p, &size)) {
    fclose(in_p);
    return 2;
  }
  printf("proccessed file size: %li\n", size);

  if (fclose(in_p)) {
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

  if (fseek(fp, cur, SEEK_END) != 0) {
    perror("fseek");
    return false;
  }
  *size = size_local;
  return true;
}
