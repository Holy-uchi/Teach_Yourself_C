#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#define BUFF_SIZE 4096

bool filesize(FILE *fp, long *out_size);

int main(void) {
  FILE *fp = fopen("data/Chap10/input_40.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  long size = 0;
  if (!filesize(fp, &size)) {
    fclose(fp);
    return 2;
  }

  if (size > BUFF_SIZE) {
    fprintf(stderr, "File size too large\n");
    fclose(fp);
    return 2;
  }

  unsigned char buff[BUFF_SIZE];
  rewind(fp);
  size_t n = fread(buff, 1, (size_t)size, fp);

  if (ferror(fp)) {
    perror("fread");
    fclose(fp);
    return 2;
  }

  if (n != (size_t)size) {
    fprintf(stderr, "short read: expected %zu, got %zu\n", (size_t)size, n);
    fclose(fp);
    return 2;
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  printf("read %zu bytes\n", n);
  return 0;
}

bool filesize(FILE *fp, long *out_size) {
  long cur = ftell(fp);
  if (cur == -1L) {
    perror("ftell");
    return false;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    perror("fseek");
    return false;
  }

  long size = ftell(fp);
  if (size == -1L) {
    perror("ftell");
    return false;
  }

  if (fseek(fp, cur, SEEK_SET) != 0) {
    perror("fseek");
    return false;
  }

  *out_size = size;
  return true;
}
