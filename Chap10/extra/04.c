// 第4問：行番号を付けて表示。
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(void) {
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }
  
  int line = 0;
  char buff[256];
  while(fgets(buff, sizeof(buff), fp) != NULL) {
    fprintf(stdout, "%i: %s", ++line, buff);
  }


  if (ferror(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    fclose(fp);
    return 1;
  }

  if (fclose(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }

  return 0;
}
