#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_18.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  char buff[256];
  char name[8];
  int age;
  int line_no = 0;
  int success = 0;

  while (fgets(buff, sizeof(buff), in_p) != NULL) {
    int rc;
    line_no++;
    rc = sscanf(buff, "%7s %i", name, &age);

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
