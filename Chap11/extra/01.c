#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int make_copy(const char *s, char **out);

int main(void) {
  const char *src_sentence = "hello, world!";
  char *copied = NULL;

  int rc = make_copy(src_sentence, &copied);
  if (rc != 0) {
    return 2;
  }

  printf("%s\n", copied);

  free(copied);
  return 0;
}

int make_copy(const char *s, char **out) {
  if (!s || !out || *out != NULL) {
    return EINVAL;
  }
  size_t len = strlen(s);
  char *tmp = malloc(len + 1);
  if (!tmp) {
    return ENOMEM;
  }
  memcpy(tmp, s, len + 1);
  *out = tmp;
  return 0;
}
