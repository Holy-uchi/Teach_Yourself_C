#include "lisp-parser.h"
#include "lisp-tokenizer.h"
#include "list-processor.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

int main(void) {
  int rc = 0;
  char buf[256];
  LineVec segments;
  lv_init(&segments);
  TokenVec tokens;
  tv_init(&tokens);
  Value *value = NULL;

  while (fgets(buf, 256, stdin) != NULL) {
    rc = segment_string(buf, &segments);
    if (rc != 0) {
      goto cleanup;
    }
    int rc_tokenizer = tokenize_segments(&segments, &tokens);
    if (rc_tokenizer != 0) {
      printf("tokenize failed: %s\n", strerror(rc_tokenizer));
      lv_destroy(&segments);
      continue;
    }

    // tokensをパースしていく
    size_t pos = 0;
    int rc_parser = parse_list(&tokens, &pos, &value);
    if (rc_parser != 0) {
      printf("parse failed: %s\n", strerror(rc_parser));
      tv_destroy(&tokens);
      lv_destroy(&segments);
      continue;
    }

    int32_t result;
    int rc_eval = lisp_eval(&value->as.cons, &result);
    if (rc_eval != 0) {
      printf("eval failed: %s\n", strerror(rc_eval));
      tv_destroy(&tokens);
      lv_destroy(&segments);
      value_delete(&value, true);
      continue;
    }
    printf("%i\n", result);

    tv_destroy(&tokens);
    lv_destroy(&segments);
    value_delete(&value, true);
  }

cleanup:
  if (rc) {
    fprintf(stderr, "%s\n", strerror(rc));
  }
  value_delete(&value, true);
  lv_destroy(&segments);
  tv_destroy(&tokens);
  return rc;
}
