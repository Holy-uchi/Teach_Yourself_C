#include <stdbool.h>
#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/input.txt", "rb");
  if (!in_p) { perror("fopen input"); return 2;}

  FILE *out_p = fopen("data/output.txt", "wb");
  if (!out_p) { perror("fopen output"); fclose(in_p); return 2; }

  int ch;
  int num_lines = 1;
  bool new_line = true;
  
  while ((ch = fgetc(in_p)) != EOF) {
    if (new_line) {
      if (fprintf(out_p, "%i: ", num_lines++) <0 ) {
        perror("write output");
        fclose(in_p);
        fclose(out_p);
        return 2;
      }
      new_line = false;
    }

    if (ch == '\n') {new_line = true;}

    if (fputc(ch, out_p) == EOF) {
      if (ferror(out_p)) perror("write output");
      else perror("fputc");
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
