// 第一引数の文字列から最初の単語を切り出して返す

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_CAP 16

int get_token(const char *s, char **out_tok) {
  if (!s || !out_tok || *out_tok != NULL) {
    return EINVAL;
  }

  size_t cap = INITIAL_CAP;
  size_t last = 0;
  char *tmp = malloc(INITIAL_CAP);
  if (!tmp) {
    return ENOMEM;
  }

  for (size_t i = 0; s[i] != '\0'; i++) {
    unsigned char ch = (unsigned char)s[i];
    if (last == 0 && isspace(ch)) {
      continue;
    } else if (last > 0 && isspace(ch)) {
      break;
    }

    if (last + 1 >= cap) {
      char *ntmp = realloc(tmp, cap * 2);
      if (!ntmp) {
        free(tmp);
        return ENOMEM;
      }
      cap *= 2;
      tmp = ntmp;
    }
    tmp[last++] = ch;
  }

  tmp[last] = '\0';
  *out_tok = tmp;
  return 0;
}

int main(void) {
  const char *hello = "Hello World!";
  char *token = NULL;

  int rc = get_token(hello, &token);
  if (rc != 0) {
    fprintf(stderr, "get_token failed: %s\n", strerror(rc));
    return 1;
  }

  puts(token);
  free(token);
  return 0;
}
