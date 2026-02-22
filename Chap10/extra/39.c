#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#define ARRAY_LEN 100

int main(void) {
  FILE *fp = fopen("data/Chap10/39.bin", "wb+");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  int a[ARRAY_LEN];
  for (int i = 0; i < ARRAY_LEN; i++) {
    a[i] = i * 13;
  }

  size_t n = fwrite(a, sizeof(a[0]), ARRAY_LEN, fp);
  if (n < ARRAY_LEN) {
    if (ferror(fp)) {
      perror("fwrite");
      fclose(fp);
      return 2;
    } else {
      fprintf(stderr, "unexpected error");
      fclose(fp);
      return 2;
    }
  }

  rewind(fp);

  int b[ARRAY_LEN];
  size_t m = fread(b, sizeof(b[0]), ARRAY_LEN, fp);
  if (m < ARRAY_LEN) {
    if (ferror(fp)) {
      perror("fread");
      fclose(fp);
      return 2;
    } else {
      fprintf(stderr, "unexpected error");
      fclose(fp);
      return 2;
    }
  }

  // 一致チェック
  bool same = true;
  for (int i = 0; i < ARRAY_LEN; i++) {
    if (a[i] != b[i]) {
      same = false;
      break;
    }
  }

  if (same) {
    printf("OK\n");
  } else {
    fprintf(stderr, "Not Same\n");
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
