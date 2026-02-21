#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_24.txt", "rb");
  if (!in_p) {
    perror("fopen");
    return 2;
  }

  int rc = 0;
  int a;
  int records = 0;

  while ((rc = fscanf(in_p, "%d", &a)) == 1) {
    // "%d" は0文字以上の空白(\nを含む)+整数にマッチする
    if (records > 0) {
      printf(", ");
    }
    records++;
    printf("%d", a);
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    return 2;
  }
  printf("\n");

  if (rc == EOF) {
    fclose(in_p);
    return 0;
  }

  fprintf(stderr, "parse failed after records %i", records);
  fclose(in_p);
  return 2;
}
