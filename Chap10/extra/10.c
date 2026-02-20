#include <string.h>
#include <stdio.h>

#define MAX_LINES 100
#define MAX_LEN 256

int main(void) {
  FILE *fp = fopen("data/input.txt", "rb");
  if (!fp) {
    perror("fopen");
    return 2;
  }

  char lines[MAX_LINES][MAX_LEN];
  char buff[MAX_LEN];
  int n = 0;

  while (fgets(buff, sizeof(buff), fp) != NULL) {
    if (n >= MAX_LINES) {
      fprintf(stderr, "Too many lines. Limit:%i\n", MAX_LINES);
      fclose(fp);
      return 2;
    }

    strcpy(lines[n], buff);
    n++;
  }

  if (ferror(fp)) {
    perror("fgets");
    fclose(fp);
    return 2;
  }

  for (int i = n - 1; i >= 0; i--) {
    fputs(lines[i], stdout);
    if (ferror(stdout)) {
      perror("stdout");
      fclose(fp);
      return 2;
    }
  }

  if (fclose(fp)) {
    perror("fclose");
    return 2;
  }

  return 0;
}
