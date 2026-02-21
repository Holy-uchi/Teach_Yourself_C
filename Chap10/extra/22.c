#include <stdbool.h>
#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_22.txt", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  int success = 0;
  int a;
  int b;
  int rc = 0;
  while ((rc = fscanf(in_p, " %d , %d", &a, &b)) == 2) {
    success++;
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    return 2;
  }

  if (rc == EOF) {
    printf("success: %i\n", success);
    fclose(in_p);
    return 0;
  }

  fprintf(stderr, "invalid format after success=%d (rc=%d)\n", success, rc);
  fclose(in_p);
  return 2;
}
