#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *fp;
  if (argc > 1) {
    fp = fopen(argv[1], "r");
    if (!fp) {
      fprintf(stderr, "%s\n", strerror(errno));
      exit(1);
    }
  } else {
    fp = stdin;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (fputc(ch, fp) == EOF) {
      fprintf(stderr, "%s\n", strerror(errno));
      return 1;
    }
  }

  if (argc > 1) {
    if (fclose(fp)) {
      fprintf(stderr, "%s\n", strerror(errno));
    }
  }
}
