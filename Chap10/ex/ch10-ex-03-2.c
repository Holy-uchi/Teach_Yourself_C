#include <stdbool.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  FILE *in_fp;
  FILE *out_fp;
  bool need_close_outfp = false;

  if (argc <= 1) {
    fprintf(stderr, "enter input file\n");
    return 2;
  } else if (argc == 2) {
    in_fp = fopen(argv[1], "rb");
    if (!in_fp) {
      perror("fopen input file");
      return 2;
    }
    out_fp = stdout;
    if (!out_fp) {
      perror("stdout");
      return 2;
    }
  } else {
    in_fp = fopen(argv[1], "rb");
    if (!in_fp) {
      perror("fopen input file");
      return 2;
    }
    out_fp = fopen(argv[2], "wb");
    if (!out_fp) {
      perror("fopen output file");
      return 2;
    }
    need_close_outfp = true;
  }

  char buff[256];
  int line_no;
  char prefecture[16];
  int population;

  // ヘッダーの出力
  if (fprintf(out_fp, "項番 都道府県 人口\n") < 0) {
    if (ferror(out_fp)) {
      perror("fprintf output");
    } else {
      fprintf(stderr, "unexpected error while fprint\n");
    }
    fclose(in_fp);
    if (need_close_outfp) {
      fclose(out_fp);
    }
    return 2;
  }

  while (fgets(buff, sizeof(buff), in_fp)) {
    int n = sscanf(buff, " %d , %15[^,] , %*[^,] , %d", &line_no, prefecture, &population);
    if (n == 3) {
      if (fprintf(out_fp, "%4d %15s %20d\n", line_no, prefecture, population) < 0) {
        if (ferror(out_fp)) {
          perror("fprintf output");
        } else {
          fprintf(stderr, "unexpected error while fprint\n");
        }
        fclose(in_fp);
        if (need_close_outfp) {
          fclose(out_fp);
        }
        return 2;
      }
    }
  }

  if (fclose(in_fp)) {
    perror("fclose input file");
  }
  if (need_close_outfp && fclose(out_fp)) {
    perror("fclose output file");
    return 2;
  }

  return 0;
}
