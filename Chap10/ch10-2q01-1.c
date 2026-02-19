#include <stdio.h>

int main(void) {

  FILE *fp = fopen("all_a.txt", "wb");

  //    char buff[129];
  //    for (int i = 0; i < 128; i++) {
  //      buff[i] = 'a';
  //    }
  //    buff[128] = '\0';
  //    fwrite(buff, sizeof(char), 128, fp);

  for (int i = 0; i < 128; i++) {
    fputc('a', fp);
  }

  fclose(fp);
}
