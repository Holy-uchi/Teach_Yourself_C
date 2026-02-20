// 第５問
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void){
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 2;
  }

  int num_empty_line = 0;
  char buff[256];
  while(fgets(buff, sizeof(buff), fp) != NULL) {
    size_t len = strlen(buff);
    
    // 行末の改行を落とす(\nと\r\n対応)
    if (len > 0 && buff[len - 1] == '\n') {
      buff[--len] = '\0';
    }
    if (len > 0 && buff[len - 1] == '\r') {
      buff[--len] = '\0';
    }

    // 改行を落とした結果、空なら「空行」
    if (len == 0) {
      num_empty_line++;
    }
  }

  if(ferror(fp)) {
    perror("fgets");
    fclose(fp);
    return 2;
  }

  printf("%i\n", num_empty_line);

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }
}
