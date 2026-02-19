#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void) {
  FILE *fp = fopen("./test.txt", "r");
  if (!fp) {
    fprintf(stderr, "failed to open test.txt, cause:%s\n", strerror(errno));
    return 1;
  }

  char name[32];
  int born;
  int died;
  char buffer[128];

  while (fgets(buffer, sizeof buffer, fp)) {
    int ret = sscanf(buffer, "%*i,%31[^,],%i,%i", name, &born, &died);
    if (ret != 3) {
      continue;
    }
    printf("%-11s %4d-%4d\n", name, born, died);
  }
  fclose(fp);
}
