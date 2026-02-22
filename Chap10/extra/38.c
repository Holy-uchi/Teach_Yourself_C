#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_38.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }
  FILE *out_p = fopen("data/Chap10/output_38.txt", "wb");
  if (!out_p) {
    perror("fopen output");
    fclose(in_p);
    return 2;
  }

  size_t total_bytes = 0;
  unsigned char buff[256];

  for (;;) {
    size_t n = fread(buff, 1, sizeof(buff), in_p);
    if (n > 0) {
      total_bytes += n;
      size_t m = fwrite(buff, 1, n, out_p);
      if (m != n) {
        perror("fwrite error");
        fclose(in_p);
        fclose(out_p);
        return 2;
      }
    }
    if (n < sizeof(buff)) {
      if (ferror(in_p)) {
        perror("fread error");
        fclose(in_p);
        fclose(out_p);
        return 2;
      }
      break;
    }
  }

  printf("total bytes: %li\n", total_bytes);

  if (fclose(in_p)) {
    perror("fclose input");
  }
  if (fclose(out_p)) {
    perror("fclose output");
    return 2;
  }

  return 0;
}
