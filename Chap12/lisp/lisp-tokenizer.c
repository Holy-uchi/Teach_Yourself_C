#include "lisp-tokenizer.h"
#include "list-processor.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void tv_init(TokenVec *tv) {
  tv->p = NULL;
  tv->len = 0;
  tv->cap = 0;
}

void tv_destroy(TokenVec *tv) {
  if (!tv) {
    return;
  }
  free(tv->p);
  tv->p = NULL;
  tv->len = 0;
  tv->cap = 0;
}

int tv_reserve(TokenVec *tv, size_t min_size) {
  if (!tv) {
    return EINVAL;
  }

  if (tv->cap >= min_size) {
    return 0;
  }

  int rc = 0;
  LispToken *tmp = NULL;
  size_t ncap = tv->cap ? tv->cap : 8;

  while (ncap < min_size) {
    if (ncap > SIZE_MAX / 2) {
      rc = EOVERFLOW;
      goto cleanup;
    }
    ncap *= 2;
  }

  if (ncap > SIZE_MAX / sizeof(LispToken)) {
    rc = EOVERFLOW;
    goto cleanup;
  }

  tmp = realloc(tv->p, sizeof(LispToken) * ncap);
  if (!tmp) {
    rc = ENOMEM;
    goto cleanup;
  }

  tv->p = tmp;
  tmp = NULL;
  tv->cap = ncap;

cleanup:
  free(tmp);
  return rc;
}

int tv_push(TokenVec *tv, LispToken token) {
  if (!tv) {
    return EINVAL;
  }
  int rc;
  rc = tv_reserve(tv, tv->len + 1);
  if (rc != 0) {
    return rc;
  }

  tv->p[tv->len] = token;
  tv->len++;

  return 0;
}

int segment_string(const char *buf, LineVec *lv_out) {
  if (!buf || !lv_out || lv_out->p != NULL || lv_out->len != 0 || lv_out->cap != 0) {
    return EINVAL;
  }
  lv_init(lv_out); // 出力を初期化状態にしておく

  int rc = 0;
  LineVec lv_local;
  lv_init(&lv_local);
  size_t anchor = 0;
  size_t last = 0;
  unsigned char uc;
  char *atom = NULL;

  bool in_atom = false;
  while ((uc = buf[last]) != '\0') {
    if (in_atom) {
      if (isspace(uc) || uc == '(' || uc == ')') {
        size_t atom_len = last - anchor;
        atom = malloc(atom_len + 1); // '\0' の+1
        if (!atom) {
          rc = ENOMEM;
          goto cleanup;
        }
        memcpy(atom, &buf[anchor], atom_len);
        atom[atom_len] = '\0';

        rc = lv_push(&lv_local, atom);
        if (rc != 0) {
          goto cleanup;
        }
        atom = NULL;
        in_atom = false;
        // '('または')'のときはそれ自体もpush
        if (uc == '(' || uc == ')') {
          atom = malloc(2);
          if (!atom) {
            rc = ENOMEM;
            goto cleanup;
          }
          atom[0] = uc;
          atom[1] = '\0';
          rc = lv_push(&lv_local, atom);
          if (rc != 0) {
            goto cleanup;
          }
          atom = NULL;
        }
      } else {
        // 何もしない
      }
    } else {
      if (uc == '(' || uc == ')') {
        atom = malloc(2);
        if (!atom) {
          rc = ENOMEM;
          goto cleanup;
        }
        atom[0] = uc;
        atom[1] = '\0';
        rc = lv_push(&lv_local, atom);
        if (rc != 0) {
          goto cleanup;
        }
        atom = NULL;
      } else if (!isspace(uc)) {
        // atom開始
        anchor = last;
        in_atom = true;
      }
    }
    last++;
  }

  if (in_atom) {
    size_t atom_len = last - anchor;
    atom = malloc(atom_len + 1); // '\0' の+1
    if (!atom) {
      rc = ENOMEM;
      goto cleanup;
    }
    memcpy(atom, &buf[anchor], atom_len);
    atom[atom_len] = '\0';
    rc = lv_push(&lv_local, atom);
    if (rc != 0) {
      goto cleanup;
    }
    atom = NULL;
  }

  *lv_out = lv_local;
  lv_init(&lv_local);

cleanup:
  lv_destroy(&lv_local);
  return rc;
}

int tokenize_segments(const LineVec *segments, TokenVec *tv_out) {
  if (!segments || !tv_out) {
    return EINVAL;
  }

  // tv_outは初期化直後を要求
  if (tv_out->p != NULL || tv_out->len != 0 || tv_out->cap != 0) {
    return EINVAL;
  }

  tv_init(tv_out);

  int rc = 0;
  TokenVec tv_local;
  tv_init(&tv_local);

  // 成功するならば、tv_out->len == segments->len
  rc = tv_reserve(&tv_local, segments->len);
  if (rc != 0) {
    goto cleanup;
  }

  for (size_t i = 0; i < segments->len; i++) {
    LispToken token = {0};
    if (!strcmp(segments->p[i], "+")) {
      token.type = TOK_OPERATOR;
      token.as.op = OP_ADD;
    } else if (!strcmp(segments->p[i], "-")) {
      token.type = TOK_OPERATOR;
      token.as.op = OP_SUB;
    } else if (!strcmp(segments->p[i], "*")) {
      token.type = TOK_OPERATOR;
      token.as.op = OP_MUL;
    } else if (!strcmp(segments->p[i], "/")) {
      token.type = TOK_OPERATOR;
      token.as.op = OP_DIV;
    } else if (!strcmp(segments->p[i], "(")) {
      token.type = TOK_LPAREN;
    } else if (!strcmp(segments->p[i], ")")) {
      token.type = TOK_RPAREN;
    } else {
      // 数字としてパースできないならinvalid
      int32_t num;
      rc = safe_strtoi32(segments->p[i], &num);
      if (rc != 0) {
        goto cleanup;
      }
      token.type = TOK_NUMBER;
      token.as.number = num;
    }
    rc = tv_push(&tv_local, token);
    if (rc != 0) {
      goto cleanup;
    }
  }

  *tv_out = tv_local;
  tv_init(&tv_local); // cleanup で破壊されないように初期化

cleanup:
  tv_destroy(&tv_local);
  return rc;
}

int safe_strtoi32(const char *str, int32_t *out) {
  if (!str || !out) {
    return EINVAL;
  }
  char *end;
  errno = 0;

  long result = strtol(str, &end, 10);
  if (str == end) {
    return EINVAL;
  } else if (errno) {
    // Long型のオーバーフロー
    return errno;
  }

  // 末尾の空白をスキップ
  while (isspace((unsigned char)(*end))) {
    end++;
  }
  if (*end != '\0') {
    // 数字以外を検知
    return EINVAL;
  }

  if (result > INT32_MAX || result < INT32_MIN) {
    return EOVERFLOW;
  }

  *out = (int32_t)result;
  return 0;
}
