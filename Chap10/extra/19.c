#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

bool parse_line(const char *line, int *a, int *b);

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_19.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  char buff[256];
  int x;
  int y;
  int line_no = 0;
  int success = 0;

  while (fgets(buff, sizeof(buff), in_p)) {
    line_no++;
    bool result = parse_line(buff, &x, &y);
    if (!result) {
      fprintf(stderr, "parse failed at line %i: %s", line_no, buff);
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

bool parse_line(const char *line, int *a, int *b) {
  int n = 0;
  int rc = sscanf(line, " %d , %d %n", a, b, &n);
  if (rc != 2) {
    return false;
  }

  for (const unsigned char *p = (const unsigned char *)line + n; *p; p++) {
    if (*p == '\n') {
      break;
    }
    if (!isspace(*p)) {
      return false;
    }
  }
  return true;
}
