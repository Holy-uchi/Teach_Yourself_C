#include <stdbool.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *fp;
  bool need_close = false;
  if (argc >= 2) {
    fp = fopen(argv[1], "rb");
    need_close = true;
  } else {
    fp = stdin;
  }

  if (!fp) {
    perror("fopen");
    return 2;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (putchar(ch) == EOF) {
      perror("putchar");
      if (need_close) {
        fclose(fp);
      }
      return 2;
    }
  }

  if (ferror(fp)) {
    perror("fgetc");
    if (need_close) {
      fclose(fp);
    }
    return 2;
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
