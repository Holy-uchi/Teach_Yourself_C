#include <stdbool.h>
#include <stdio.h>

bool filesize(FILE *fp, long *size);

int main(void) {
  FILE *fp = fopen("data/Chap10/input_31.txt", "w+");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  long size = 0;
  if (!filesize(fp, &size)) {
    fclose(fp);
    return 2;
  }
  printf("initial file size: %li bytes\n", size);

  fprintf(fp, "something\n");
  fflush(fp);

  if (!filesize(fp, &size)) {
    fclose(fp);
    return 2;
  }
  printf("after file size: %li bytes\n", size);

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
