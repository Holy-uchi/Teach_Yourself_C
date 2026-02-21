#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

int main(void) {

  FILE *in_p = fopen("data/input.txt", "rb");
  if (!in_p) {
    perror("open input");
    return 2;
  }

  FILE *out_p = fopen("data/result.txt", "wb");
  if (!out_p) {
    perror("open output");
    fclose(in_p);
    return 2;
  }

  // ループの度bytesをインクリメント
  // '\n'のたびnum_nlをインクリメント
  // '\n'でis_last_nlをtrue, ループの先頭でfalse
  // ループの後、bytes > 0 && !is_last_nl なら最終行が改行なしなので+1

  size_t bytes = 0;
  int num_nl = 0;
  bool is_last_nl = false;
  int ch;

  while ((ch = fgetc(in_p)) != EOF) {
    bytes++;

    if (ch == '\n') {
      num_nl++;
      is_last_nl = true;
    } else {
      is_last_nl = false;
    }
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    fclose(out_p);
    return 2;
  }

  bool end_without_nl = bytes > 0 && !is_last_nl;
  int line_count = end_without_nl ? num_nl + 1 : num_nl;

  if (fprintf(out_p, "Line: %i, Bytes: %zu\n", line_count, bytes) < 0) {
    perror("write out");
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
