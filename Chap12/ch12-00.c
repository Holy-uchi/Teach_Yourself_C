// 第１問
#include <_string.h>
#include <stdint.h>

struct ch12q0 {
  int32_t i32;
  char *cp;
  char ch;
  int16_t fa[];
};

typedef struct {
  int32_t i32;
  char *cp;
  char ch;
  int16_t fa[];
} CH12Q0;

struct {
  int32_t i32;
  char *cp;
  char ch;
  int16_t fa[];
} ch12q0;

// 第２問

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int argc;
  char *argv[];
} ARG;

void print_arg(ARG *arg) {
  for (int i = 0; i < arg->argc; i++) {
    puts(arg->argv[i]);
  }
}

int main(int argc, char *argv[]) {
  ARG *pa;
  pa = malloc(sizeof(ARG) + sizeof(char *) * (argc));
  if (!pa) {
    return 1;
  }
  pa->argc = argc;
  for (int i = 0; i < argc; i++) {
    pa->argv[i] = strdup(argv[i]);
  }

  print_arg(pa);

  for (int i = 0; i < argc; i++) {
    free(pa->argv[i]);
  }
  free(pa);
}
