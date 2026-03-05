#include "my-utils.h"
#include "line-vector.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

// XXX: 全体的にクソ適当

// StrBufはlen+1 <= cap
void sb_init(StrBuf *sb) {
  sb->p = NULL;
  sb->len = 0;
  sb->cap = 0;
}

void sb_destroy(StrBuf *sb) {
  free(sb->p);
  sb->len = 0;
  sb->cap = 0;
}

// min_sizeは'\0'を考慮したサイズを呼び出し側が渡す
int sb_reserve(StrBuf *sb, size_t min_size) {
  if (!sb) {
    return EINVAL;
  }
  if (sb->cap >= min_size) {
    return 0;
  }

  int rc = 0;
  char *tmp = NULL;
  size_t ncap = sb->cap ? sb->cap : 8;
  while (ncap < min_size) {
    if (ncap > SIZE_MAX / 2) {
      rc = EOVERFLOW;
      goto cleanup;
    }
    ncap *= 2;
  }
  tmp = realloc(sb->p, ncap);
  if (!tmp) {
    rc = ENOMEM;
    goto cleanup;
  }
  sb->p = tmp;
  tmp = NULL;
  sb->cap = ncap;

cleanup:
  free(tmp);
  return rc;
}

int sb_push_char(StrBuf *sb, char ch) {
  if (!sb || ch == '\0') {
    return EINVAL;
  }
  int rc = sb_reserve(sb, sb->len + 2);
  if (rc != 0) {
    return rc;
  }
  sb->p[sb->len] = ch;
  sb->p[sb->len + 1] = '\0';
  sb->len++;

  return 0;
}

// TODO:
int sb_append(StrBuf *sb, char *str) {
  (void)sb;
  (void)str;
  return ENOSYS;
}

typedef enum {
  TOKEN_START,
  TOKEN_CONTINUE,
  TOKEN_END,
  SPACE_CONTINUE,
} PARSE_STATE;

int parse_line(const char *line, LineVec *tok_vec) {
  if (!line || !tok_vec) {
    return EINVAL;
  }
  if (tok_vec->p != NULL || tok_vec->len != 0 || tok_vec->cap != 0) {
    // NOTE: リーク防止
    return EINVAL;
  }

  int rc = 0;
  LineVec tok_local = {0};
  lv_init(&tok_local);
  StrBuf token = {0};
  sb_init(&token);
  bool last_was_space = true;

  size_t len = strlen(line);
  for (size_t i = 0; i < len; i++) {
    unsigned char uc = line[i];
    PARSE_STATE state;
    if (!last_was_space && isspace(uc)) {
      state = TOKEN_END;
    } else if (!last_was_space && !isspace(uc)) {
      state = TOKEN_CONTINUE;
    } else if (last_was_space && !isspace(uc)) {
      state = TOKEN_START;
    } else {
      state = SPACE_CONTINUE;
    }

    switch (state) {
    case TOKEN_START:
      sb_init(&token); // 念の為
      rc = sb_push_char(&token, (char)uc);
      if (rc != 0) {
        goto cleanup;
      }
      last_was_space = false;
      break;
    case TOKEN_CONTINUE:
      rc = sb_push_char(&token, (char)uc);
      if (rc != 0) {
        goto cleanup;
      }
      break;
    case TOKEN_END:
      rc = lv_push(&tok_local, token.p); // token.pの所有権を移譲
      if (rc != 0) {
        goto cleanup;
      }
      sb_init(&token);
      last_was_space = true;
      break;
    case SPACE_CONTINUE:
      break;
    }
  }
  // 最後が空白じゃなかった場合は？
  if (!last_was_space) {
    rc = lv_push(&tok_local, token.p);
    if (rc != 0) {
      goto cleanup;
    }
    sb_init(&token);
  }

  // 成功時は格納
  *tok_vec = tok_local;
  lv_init(&tok_local);

cleanup:
  sb_destroy(&token);
  lv_destroy(&tok_local);
  return rc;
}
