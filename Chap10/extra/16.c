#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/input_16.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  int rc;
  int sum = 0;
  int x;
  while ((rc = fscanf(in_p, "%d", &x)) == 1) {
    sum += x;
  }

  if (rc == EOF) {
    if (ferror(in_p)) {
      perror("read input");
      fclose(in_p);
      return 2;
    }
    // EOFで正常終了
  } else {
    // rc = 0: 変換失敗(フォーマット崩れ)
    fprintf(stderr, "parse error: expected int\n");
    fclose(in_p);
    return 2;
  }

  printf("%i\n", sum);

  if (fclose(in_p)) {
    perror("fclose input");
    return 2;
  }

  return 0;
}
