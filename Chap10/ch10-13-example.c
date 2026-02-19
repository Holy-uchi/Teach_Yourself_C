#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

int main(void) {
  FILE *fp = fopen("append-test.txt", "a");
  if (!fp) {
    puts(strerror(errno));
    return 2;
  }
  fputs("new line\n", fp);
  fclose(fp);
}
