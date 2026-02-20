#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  FILE *fp = fopen("data/ch10-3q03.csv", "rb");
  if (!fp) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }

  char buff[256];
  int num;
  char pref[32];
  int pop;
  bool first = true;

  while (fgets(buff, sizeof(buff), fp) != NULL) {
    int n = sscanf(buff, "%i,%31[^,],%*[^,],%i", &num, pref, &pop);
    if (n != 3) {
      // fprintf(stderr, "something went wrong\n");
      // return 1;
      continue;
    }
    if (first) {
      puts("項番  都道府県  人口");
      first = false;
    }
    printf("%4i %-8s %9i\n", num, pref, pop);
  }

  if (ferror(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    fclose(fp);
    return 1;
  }

  if (fclose(fp)) {
    fprintf(stderr, "%s\n", strerror(errno));
    return 1;
  }
}
