#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void){
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }

  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (fputc(ch,stdout) == EOF) {
    fprintf(stderr, "%s\n", strerror(errno));
      return 1;
    }
  }

  if(ferror(fp)) {
    // 読み取りエラー検出
    fprintf(stderr, "%s\n", strerror(errno));
  }

  if (fclose(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }
}
