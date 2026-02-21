#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

bool parse_line(const char *line, int *sum);

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_21.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  char buff[256];
  int line_no = 0;

  while (fgets(buff, sizeof(buff), in_p)) {
    line_no++;
    int sum = 0;
    if (!parse_line(buff, &sum)) {
      fprintf(stderr, "parse failed at line %i: %s", line_no, buff);
      fclose(in_p);
      return 2;
    }
    printf("line %i: %i\n", line_no, sum);
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    return 2;
  }

  if (fclose(in_p)) {
    perror("fclose input");
    return 2;
  }

  return 0;
}

bool parse_line(const char *line, int *sum) {
  int nums[3] = {0, 0, 0};
  int n0 = -1, n1 = -1, n2 = -1;

  int rc = sscanf(line, " %d %n %d %n %d %n", &nums[0], &n0, &nums[1], &n1, &nums[2], &n2);

  // 空行なら不正
  if (rc <= 0) {
    return false;
  }

  int n = (rc == 1) ? n0 : (rc == 2) ? n1 : n2;
  if (n < 0) {
    return false;
  }

  // 数字以外のゴミがあれば不正
  for (const unsigned char *p = (const unsigned char *)line + n; *p != '\0'; p++) {
    if (*p == '\n') {
      break;
    }
    if (!isspace(*p)) {
      return false;
    }
  }

  // 得られた数字の和を返す
  int sum_local = 0;
  for (int i = 0; i < rc; i++) {
    sum_local += nums[i];
  }
  *sum = sum_local;

  return true;
}
