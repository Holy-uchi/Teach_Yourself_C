#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_17.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  char buff[256];
  int x;
  int y;
  int success = 0;
  int line_no = 0;
  while (fgets(buff, sizeof(buff), in_p) != NULL) {
    line_no++;
    int rc;
    rc = sscanf(buff, "%i,%i", &x, &y);

    if (rc != 2) {
      fprintf(stderr, "parse error at line %i: %s", line_no, buff);
      fclose(in_p);
      return 2;
    }
    success++;
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    return 2;
  }

  printf("success: %i\n", success);

  if (fclose(in_p)) {
    perror("fclose input");
    return 2;
  }

  return 0;
}
