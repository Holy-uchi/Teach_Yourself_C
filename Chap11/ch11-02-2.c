#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct emp {
  int born;
  int died;
  char name[];
} EMP;
typedef int (*SORT_FUNC)(const void *, const void *);

int sort_by_name(const void *a, const void *b);
int sort_by_born(const void *a, const void *b);
int sort_by_died(const void *a, const void *b);

#define INIT_SIZE 4
#define INC_SIZE 4

int main(void) {
  FILE *fin = fopen("data/Chap11/ch11-13.data", "rb");
  if (!fin) {
    perror("open error");
    return 2;
  }
  char name[32];
  int born;
  int died;
  EMP **emps = malloc(sizeof(EMP *) * INIT_SIZE);
  size_t csize = INIT_SIZE;
  size_t last = 0;
  for (;;) {
    int ret = fscanf(fin, "%*i,%31[^,],%i,%i", name, &born, &died);
    if (ret == EOF) {
      break;
    }
    emps[last] = malloc(sizeof(EMP) + strlen(name) + 1);
    strcpy(emps[last]->name, name);
    emps[last]->born = born;
    emps[last]->died = died;
    last++;
    if (last == csize) {
      EMP **nemps = realloc(emps, sizeof(EMP *) * (csize + INC_SIZE));
      if (!nemps) {
        goto cleanup;
      }
      emps = nemps;
      csize += INC_SIZE;
    }
  }

  fclose(fin);
  // for (size_t i = 0; i < last; i++) {
  //   printf("%s %i-%i\n", emps[i]->name, emps[i]->born, emps[i]->died);
  // }
  int ch;
  if ((ch = fgetc(stdin)) == EOF) {
    perror("fgetc error");
    goto cleanup;
  }
  SORT_FUNC fp = NULL;
  if (ch == '1') {
    fp = &sort_by_born;
  } else if (ch == '2') {
    fp = &sort_by_died;
  } else {
    fp = &sort_by_name;
  }

  qsort(emps, last, sizeof(emps[0]), fp);
  for (size_t i = 0; i < last; i++) {
    printf("%s %i-%i\n", emps[i]->name, emps[i]->born, emps[i]->died);
  }

cleanup:
  for (size_t i = 0; i < last; i++) {
    free(emps[i]);
  }
  free(emps);
}

// nameでsort
int sort_by_name(const void *a, const void *b) {
  const EMP *emp_a = *(const EMP *const *)a;
  const EMP *emp_b = *(const EMP *const *)b;
  return strcmp(emp_a->name, emp_b->name);
}
// bornでsort
int sort_by_born(const void *a, const void *b) {
  const EMP *emp_a = *(const EMP *const *)a;
  const EMP *emp_b = *(const EMP *const *)b;
  return emp_a->born - emp_b->born;
}
// diedでsort
int sort_by_died(const void *a, const void *b) {
  const EMP *emp_a = *(const EMP *const *)a;
  const EMP *emp_b = *(const EMP *const *)b;
  return emp_a->died - emp_b->died;
}
