#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct emp {
  int born;
  int died;
  struct emp *next;
  char name[];
} EMP;

int main(void) {
  FILE *fin = fopen("data/Chap11/ch11-09.data", "rb");
  if (!fin) {
    perror("open error");
    return 2;
  }

  char name[32];
  int born;
  int died;
  EMP *top = NULL;
  EMP *last = NULL;
  for (;;) {
    int ret = fscanf(fin, "%*i,%31[^,],%i,%i", name, &born, &died);
    if (ret == EOF) {
      break;
    }

    EMP *emp = malloc(sizeof(EMP) + strlen(name) + 1);
    if (!emp) {
      fprintf(stderr, "no memory for %s\n", name);
      break;
    }

    strcpy(emp->name, name);
    emp->born = born;
    emp->died = died;
    emp->next = NULL;
    if (!last) {
      last = top = emp;
    } else {
      last->next = emp;
      last = emp;
    }
  }
  fclose(fin);
  for (EMP *p = top; p; p = p->next) {
    printf("%s %i-%i\n", p->name, p->born, p->died);
  }
  EMP *p = top;
  while (p) {
    EMP *np = p->next;
    free(p);
    p = np;
  }
}
