#include "lisp-tokenizer.h"
#include <stddef.h>
#include <stdio.h>

int main(void) {
  int rc = 0;
  char buf[256];
  LineVec tokens;
  lv_init(&tokens);

  while (fgets(buf, 256, stdin) != NULL) {
    rc = lisp_tokenize(buf, &tokens);
    if (rc != 0) {
      goto cleanup;
    }
    for (size_t i = 0; i < tokens.len; i++) {
      puts(tokens.p[i]);
    }
    lv_destroy(&tokens);
  }

cleanup:
  lv_destroy(&tokens);
  return rc;
}
