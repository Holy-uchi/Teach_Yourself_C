#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readline(char *string, );

#define STR_INIT_LEN 32
#define STR_ADD_LEN 32
typedef struct {
  char *value;
  size_t len;
  size_t cap;
} STRING;

STRING string_new(void) {}

bool string_from(char *src, STRING *dest) {
  bool ok = false;
  size_t len = strlen(src);
  STRING tmp = {0};
  tmp.value = malloc(sizeof(char) * STR_INIT_LEN);
  tmp.cap = STR_INIT_LEN;
}
