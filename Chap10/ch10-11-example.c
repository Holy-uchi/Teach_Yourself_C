#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
  if (argc != 2) {
    puts("ファイル名を指定してください。");
    return 1;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp) {
    char buffer[128];
    while (fgets(buffer, 127, fp)) {
      printf("%s", buffer);
    }
    fclose(fp);
  } else {
    puts(strerror(errno));
  }
}
