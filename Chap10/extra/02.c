#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void)
{
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }
  int lines = 0;
  int ch;
  int saw_any = 0;
  int last_was_nl = 0;
  
  while ((ch = fgetc(fp)) != EOF) {
    saw_any = 1;
    last_was_nl = (ch == 'n');
    if (ch == '\n') {
      lines++;
    }
  }

  if (ferror(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }

  if (saw_any && !last_was_nl) lines++;
  printf("%i\n", lines);

  if (fclose(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }
}
