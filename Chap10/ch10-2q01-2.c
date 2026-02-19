#include <ctype.h>
#include <stdio.h>

int main(void) {
  FILE *fp = fopen("all_a.txt", "rb+");

  int pos[4] = {8, 16, 32, 64};

  for (int i = 0; i < 4; i++) {
    long target = pos[i];
    if (fseek(fp, target - 1, SEEK_SET) != 0) {
      break;
    }
    int ch = fgetc(fp);
    ch = toupper(ch);
    fseek(fp, -1, SEEK_CUR);
    fputc(ch, fp);
  }

  fclose(fp);
}
