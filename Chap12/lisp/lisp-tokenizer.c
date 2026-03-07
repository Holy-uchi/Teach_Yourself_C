#include "lisp-tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

void lv_init(LineVec *lv) {
  lv->p = NULL;
  lv->len = 0;
  lv->cap = 0;
}

void lv_destroy(LineVec *lv) {
  for (size_t i = 0; i < lv->len; i++) {
    free(lv->p[i]);
  }
  free(lv->p);
  lv->p = NULL;
  lv->len = 0;
  lv->cap = 0;
}

int lv_reserve(LineVec *lv, size_t min_size) {
  if (!lv) {
    return EINVAL;
  }

  if (lv->cap >= min_size) {
    return 0;
  }

  int rc = 0;
  char **tmp = NULL;
  size_t ncap = lv->cap ? lv->cap : 8;
  while (ncap < min_size) {
    if (ncap > SIZE_MAX / 2) {
      rc = EOVERFLOW;
      goto cleanup;
    }
    ncap *= 2;
  }

  if (ncap > SIZE_MAX / sizeof(char *)) {
    rc = EOVERFLOW;
    goto cleanup;
  }

  tmp = realloc(lv->p, sizeof(char *) * ncap);
  if (!tmp) {
    rc = ENOMEM;
    goto cleanup;
  }

  lv->p = tmp;
  tmp = NULL;
  lv->cap = ncap;

cleanup:
  free(tmp);
  return rc;
}

int lv_push(LineVec *lv, char *str) {
  if (!lv || !str) {
    return EINVAL;
  }
  int rc;

  rc = lv_reserve(lv, lv->len + 1);
  if (rc != 0) {
    return rc;
  }

  lv->p[lv->len] = str;
  lv->len++;

  return 0;
}

int lisp_tokenize(const char *buf, LineVec *lv_out) {
  if (!buf || !lv_out || lv_out->p != NULL) {
    return EINVAL;
  }
  lv_init(lv_out); // 出力を初期化状態にしておく

  int rc = 0;
  LineVec lv_local;
  lv_init(&lv_local);
  size_t anchor = 0;
  size_t last = 0;
  unsigned char uc;
  char *token = NULL;

  bool in_atom = false;
  while ((uc = buf[last]) != '\0') {
    if (in_atom) {
      if (isspace(uc) || uc == '(' || uc == ')') {
        size_t tok_len = last - anchor;
        token = malloc(tok_len + 1); // '\0' の+1
        if (!token) {
          rc = ENOMEM;
          goto cleanup;
        }
        memcpy(token, &buf[anchor], tok_len);
        token[tok_len] = '\0';

        rc = lv_push(&lv_local, token);
        if (rc != 0) {
          goto cleanup;
        }
        token = NULL;
        in_atom = false;
        // '('または')'のときはそれ自体もpush
        if (uc == '(' || uc == ')') {
          token = malloc(2);
          if (!token) {
            rc = ENOMEM;
            goto cleanup;
          }
          token[0] = uc;
          token[1] = '\0';
          rc = lv_push(&lv_local, token);
          if (rc != 0) {
            goto cleanup;
          }
          token = NULL;
        }
      } else {
        // 何もしない
      }
    } else {
      if (uc == '(' || uc == ')') {
        token = malloc(2);
        if (!token) {
          rc = ENOMEM;
          goto cleanup;
        }
        token[0] = uc;
        token[1] = '\0';
        rc = lv_push(&lv_local, token);
        if (rc != 0) {
          goto cleanup;
        }
        token = NULL;
      } else if (!isspace(uc)) {
        // atom開始
        anchor = last;
        in_atom = true;
      }
    }
    last++;
  }

  if (in_atom) {
    size_t tok_len = last - anchor;
    char *token = malloc(tok_len + 1); // '\0' の+1
    if (!token) {
      rc = ENOMEM;
      goto cleanup;
    }
    memcpy(token, &buf[anchor], tok_len);
    token[tok_len] = '\0';
    rc = lv_push(&lv_local, token);
    if (rc != 0) {
      goto cleanup;
    }
    token = NULL;
  }

  *lv_out = lv_local;
  lv_init(&lv_local);

cleanup:
  lv_destroy(&lv_local);
  return rc;
}
