#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/input.txt", "rb");
  if (!in_p) { perror("fopen input"); return 2;}

  FILE *out_p = fopen("data/output.txt", "wb");
  if (!out_p) { perror("fopen output"); fclose(in_p); return 2; }

  char buff[256];
  while (fgets(buff, sizeof(buff), in_p)) {
    if (fputs(buff, out_p) == EOF) {
      if (ferror(out_p)) perror("write output");
      else perror("fputs");
      fclose(in_p);
      fclose(out_p);
      return 2;
    }
  }

  if (ferror(in_p)) {
    perror("read input");
    fclose(in_p);
    fclose(out_p);
    return 2;
  }

  if (fclose(in_p) != 0) { perror("fclose input"); }
  if (fclose(out_p) != 0) { perror("fclose output"); return 2; }

  return 0;
}
