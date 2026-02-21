#include <ctype.h>
#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/Chap10/input_23.csv", "rb");
  if (!in_p) {
    perror("fopen input");
    return 2;
  }

  int rc = 0;
  int a;
  int records = 0;

  //  while ((rc = fscanf(in_p, " %*d , %d , %*d", &a)) == 1) {
  //    records++;
  //    printf("%i\n", a);
  //  }

  while (1) {
    rc = fscanf(in_p, " %*d , %d , %*d", &a);

    if (rc == 1) {
      int ch;
      while ((ch = fgetc(in_p)) != EOF) {
        if (ch == '\n') {
          break;
        }
        if (!isspace((unsigned char)ch)) {
          fprintf(stderr, "parse failed after records=%d (garbage)\n", records);
          fclose(in_p);
          return 2;
        }
      }

      records++;
      printf("%d\n", a);
      continue;
    }

    break; // rc != 1 or EOF
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    return 2;
  }

  if (rc == EOF) {
    fclose(in_p);
    return 0;
  }

  fprintf(stderr, "parse failed after records %i\n", records);
  fclose(in_p);
  return 2;
}
