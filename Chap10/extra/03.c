#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void) {
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }

  int ch;
  size_t bytes = 0;
  while ((ch = fgetc(fp)) != EOF) {
    bytes++;
  }

  if (ferror(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    fclose(fp);
    return 1;
  }

  printf("%lli\n", (long long)bytes);

  if (fclose(fp)){
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }

  return 0;
}
