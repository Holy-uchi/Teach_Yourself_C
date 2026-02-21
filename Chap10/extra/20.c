// 19と同様に、末尾のゴミを検知する
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

bool parse_line(const char *line, int *a, int *b);
bool is_blank(const char *line);

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_20.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  char buff[256];
  int a;
  int b;
  int num_lines = 0;
  int success = 0;

  while (fgets(buff, sizeof(buff), in_p) != NULL) {
    num_lines++;

    if (is_blank(buff)) {
      continue;
    }

    if (parse_line(buff, &a, &b)) {
      success++;
    } else {
      fprintf(stderr, "parse failed at line %i: %s", num_lines, buff);
      fclose(in_p);
      return 2;
    }
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

bool is_blank(const char *line) {
  for (const unsigned char *p = (const unsigned char *)line; *p != '\0'; p++) {
    if (*p == '\n') {
      break;
    }
    if (!isspace(*p)) {
      return false;
    }
  }
  return true;
}

bool parse_line(const char *line, int *a, int *b) {
  int n = 0;
  int rc = sscanf(line, " %d , %d %n", a, b, &n);
  if (rc != 2) {
    return false;
  }

  for (const unsigned char *p = (unsigned char *)line + n; *p != '\0'; p++) {
    if (*p == '\n') {
      break;
    }
    if (!isspace(*p)) {
      return false;
    }
  }

  return true;
}
