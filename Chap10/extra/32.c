#include <stdbool.h>
#include <stdio.h>

bool filesize(FILE *fp, long *size);

int main(void) {
  FILE *fp = fopen("data/Chap10/input_32.txt", "ab+");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  long size = 0;
  if (!filesize(fp, &size)) {
    fclose(fp);
    return 2;
  }
  printf("initial filesize: %li bytes\n", size);

  if (fseek(fp, 0, SEEK_SET) != 0) {
    perror("fseek");
    fclose(fp);
    return 2;
  }

  if (fprintf(fp, "Will this be written to the beginning of the file?") < 0) {
    perror("fprintf");
    fclose(fp);
    return 2;
  }
  fflush(fp);

  if (!filesize(fp, &size)) {
    fclose(fp);
    return 2;
  }
  printf("filesize after: %li bytes\n", size);

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
