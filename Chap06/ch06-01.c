// 第１問
// #include <stdio.h>
//
// int main(int argc, char *argv[]) {
//   int i = 1;
//   while (i < argc) {
//     puts(argv[i]);
//     i++;
//   }
// }

// 第２問

#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc != 3) {
    puts("give 2 file names");
    return 1;
  }

  FILE *f_from = fopen(argv[1], "r");
  FILE *f_to = fopen(argv[2], "w");
  if (!f_from || !f_to) {
    puts("failed to open file");
    if (f_from)
      fclose(f_from);
    if (f_to)
      fclose(f_to);
    return 1;
  }
  char ch; // ChatGPT曰く、fgetcの返り値がintなのでintで受け取るべきらしい。:

  while ((ch = fgetc(f_from)) != EOF) {
    fputc(ch, f_to);
  }
  fclose(f_from);
  fclose(f_to);
  return 0;
}
