#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  FILE *in_p = fopen("data/input.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  FILE *out_p = fopen("data/out_even.txt", "wb");
  if (!out_p) {
    perror("fopen output");
    fclose(in_p);
    return 2;
  }

  // 偶数行ならoutに書き込む
  // 改行されるなら行番号をインクリメント
  int line_no = 1;
  char buff[8];
  while (fgets(buff, sizeof(buff), in_p) != NULL) {
    if (line_no % 2 == 0) {
      if (fputs(buff, out_p) == EOF) {
        perror("write output");
        fclose(in_p);
        fclose(out_p);
        return 2;
      }
    }

    size_t len = strlen(buff);
    bool has_nl = len > 0 && buff[len - 1] == '\n';

    if (has_nl) {
      line_no++;
    }
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    fclose(out_p);
    return 2;
  }

  if (fclose(in_p)) {
    perror("fclose input");
  }
  if (fclose(out_p)) {
    perror("fclose output");
    return 2;
  }

  return 0;
}
