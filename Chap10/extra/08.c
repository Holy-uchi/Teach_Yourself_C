#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  char buff[256];
  while (fgets(buff, sizeof(buff), fp) != NULL) {
    size_t len = strlen(buff);

    if (len > 0 && buff[len - 1] == '\n') buff[--len] = '\0';
    if (len > 0 && buff[len - 1] == '\r') buff[--len] = '\0';

    if (len >= 10) {
      if (fprintf(stdout, "%s\n", buff) < 0) {
        perror("fprintf");
        fclose(fp);
        return 2;
      }
    }
  }

  if (ferror(fp)) {
    perror("fgets");
    fclose(fp);
    return 2;
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }
}
