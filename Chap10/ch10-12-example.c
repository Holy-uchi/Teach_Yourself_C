#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("ファイル名を指定してください。");
    return 1;
  }

  size_t fnlen = strlen(argv[1]) + 5;
  char backupname[fnlen];
  snprintf(backupname, fnlen, "%.*s.bak", (int)strlen(argv[1]), argv[1]);
  FILE *src = fopen(argv[1], "r");
  if (!src) {
    puts(strerror(errno));
    return 2;
  }
  FILE *dest = fopen(backupname, "w");
  if (!dest) {
    puts(strerror(errno));
    return 2;
  }
  char ch;
  while ((ch = fgetc(src)) != EOF) {
    if (fputc(ch, dest) == EOF) {
      puts(strerror(errno));
      return 2;
    }
  }
  fclose(dest);
  fclose(src);
}
