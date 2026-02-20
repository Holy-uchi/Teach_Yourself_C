#include <stdio.h>

int main(void) {
  FILE *in_p = fopen("data/input.txt", "rb");
  if (!in_p) { perror("fopen input"); return 2;}

  FILE *out_p = fopen("data/out_even.txt", "wb");
  if (!out_p) { perror("fopen output"); fclose(in_p); return 2; }

  

}
