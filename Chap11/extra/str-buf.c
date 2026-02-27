// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g sb_exercise.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined sb_exercise.c &&
// ./a.out

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

typedef struct {
  char *p;    // NUL終端を維持する
  size_t len; // 現在の文字数（NULは含めない）
  size_t cap; // 確保容量（バイト数、NULを含む領域）
} StrBuf;

/************ ここから：所与（触らない） ************/
static void sb_init(StrBuf *sb) {
  if (!sb)
    return;
  sb->p = NULL;
  sb->len = 0;
  sb->cap = 0;
}

static bool sb_invariant_ok(const StrBuf *sb) {
  if (!sb)
    return false;
  if (sb->p == NULL)
    return sb->len == 0 && sb->cap == 0;
  if (sb->cap == 0)
    return false;
  if (sb->cap < sb->len + 1)
    return false;
  if (sb->p[sb->len] != '\0')
    return false;
  return true;
}

static int safe_add_size(size_t a, size_t b, size_t *out) {
  if (!out)
    return EINVAL;
  if (SIZE_MAX - a < b)
    return EOVERFLOW;
  *out = a + b;
  return 0;
}

static int safe_mul_size(size_t a, size_t b, size_t *out) {
  if (!out)
    return EINVAL;
  if (a != 0 && b > SIZE_MAX / a)
    return EOVERFLOW;
  *out = a * b;
  return 0;
}

static size_t max_size(size_t a, size_t b) { return (a > b) ? a : b; }

static void expect(bool cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "TEST FAIL: %s\n", msg);
    abort();
  }
}
/************ ここまで：所与 ************/

/*
 * TODO（あなたの担当）
 *
 * 契約（重要）：
 * - すべて errno 互換（0成功、非0失敗）
 * - 失敗時：sb を壊さない（= 可能な限り “strong guarantee”）
 * - realloc は必ず一時変数で受ける（失敗時に元を失わない）
 * - 不変条件：sb->p != NULL のとき sb->p[sb->len] == '\0'
 */

// 内部バッファの解放（内部資源のみ）
// - sb==NULL は no-op
// - free したら {0} に戻す
void sb_destroy(StrBuf *sb) {
  if (!sb)
    return;
  free(sb->p);
  sb->p = NULL;
  sb->len = 0;
  sb->cap = 0;
  return;
}

// 容量確保：cap >= min_cap を満たす（必要なら拡張）
// - min_cap は「NUL領域込み」の必要容量
// - 失敗時：sb不変
int sb_reserve(StrBuf *sb, size_t min_cap) {
  if (!sb) {
    return EINVAL;
  }
  if (sb->cap >= min_cap) {
    return 0;
  }
  int rc;
  size_t ncap = max_size(sb->cap, 8); // もしcap=0なら8バイトで初期化
  while (ncap < min_cap) {

    if ((rc = safe_mul_size(ncap, 2, &ncap)) != 0) {
      return rc;
    }
  }
  char *tmp = realloc(sb->p, ncap);
  if (!tmp) {
    return ENOMEM;
  }
  sb->p = tmp;
  tmp = NULL;
  sb->cap = ncap;

  return 0;
}

// 文字列追記（NUL終端文字列）
// - s==NULL は EINVAL
// - 失敗時：sb不変
int sb_append(StrBuf *sb, const char *s) {
  if (!sb || !s) {
    return EINVAL;
  }
  size_t add_len = strlen(s);
  size_t needed = 0;
  int rc = safe_add_size(sb->len, add_len, &needed);
  if (rc) {
    return rc;
  }
  size_t tmp = needed;
  needed = 0;
  rc = safe_add_size(tmp, 1, &needed);
  if (rc) {
    return rc;
  }

  rc = sb_reserve(sb, needed);
  if (rc) {
    return rc;
  }
  memcpy(sb->p + sb->len, s, add_len + 1);
  sb->len += add_len;
  return 0;
}

// nバイト追記（バイナリ寄り）
// - s==NULL && n>0 は EINVAL
// - 追記後も NUL終端を維持
// - 失敗時：sb不変
int sb_append_n(StrBuf *sb, const char *s, size_t n) {
  if (!sb) {
    return EINVAL;
  }
  if (!s && n > 0) {
    return EINVAL;
  }
  size_t needed = 0;
  int rc = safe_add_size(sb->len, n, &needed);
  if (rc) {
    return rc;
  }
  size_t tmp = needed;
  needed = 0;
  rc = safe_add_size(tmp, 1, &needed);
  if (rc) {
    return rc;
  }
  rc = sb_reserve(sb, needed); // NULL終端文を余計に確保
  if (rc) {
    return rc;
  }
  memcpy(sb->p + sb->len, s, n);
  sb->len += n;
  sb->p[sb->len] = '\0';

  return 0;
}

// printf追記（snprintf 2段階を隠蔽）
// - fmt==NULL は EINVAL
// - 失敗時：sb不変
int sb_appendf(StrBuf *sb, const char *fmt, ...) {
  // TODO
  if (!fmt) {
    return EINVAL;
  }
  va_list ap;
  va_start(ap, fmt);
  int n1 = vsnprintf(NULL, 0, fmt, ap);
  va_end(ap);

  if (n1 < 0) {
    return n1;
  }

  size_t needed = 0;
  int rc = safe_add_size(sb->len, n1, &needed);
  if (rc) {
    return rc;
  }
  size_t tmp = needed;
  needed = 0;
  rc = safe_add_size(tmp, 1, &needed);
  if (rc) {
    return rc;
  }

  rc = sb_reserve(sb, needed);
  if (rc) {
    return rc;
  }

  va_start(ap, fmt);
  int n2 = vsnprintf(sb->p + sb->len, n1 + 1, fmt, ap);
  va_end(ap);
  if (n2 < 0) {
    return n2;
  } else if (n1 != n2) {
    return EIO;
  }

  sb->len += n2;

  return 0;
}

// 所有権移譲（move/detach）
// - 返り値：呼び出し側所有の char*（free必須）または NULL
// - 呼び出し後 sb は空（{0}）になる
char *sb_detach(StrBuf *sb) {
  char *tmp = sb->p;
  sb->p = NULL;
  sb->len = 0;
  sb->cap = 0;

  return tmp;
}

/************ テスト（触らない） ************/
int main(void) {
  StrBuf sb;
  sb_init(&sb);

  expect(sb_invariant_ok(&sb), "init invariant");

  // 1) append
  int rc = sb_append(&sb, "Hello");
  expect(rc == 0, "append Hello");
  expect(sb_invariant_ok(&sb), "invariant after append");
  expect(sb.len == 5, "len==5");
  expect(strcmp(sb.p, "Hello") == 0, "content Hello");

  // 2) append (realloc triggers)
  rc = sb_append(&sb, ", ");
  expect(rc == 0, "append comma");
  rc = sb_append(&sb, "World!");
  expect(rc == 0, "append World");
  expect(strcmp(sb.p, "Hello, World!") == 0, "content Hello, World!");

  // 3) append_n (binary-ish; include '\0' inside)
  // const char raw[] = {'A', 'B', '\0', 'C'}; // 途中に'\0'を入れると後続のテストが失敗する.
  const char raw[] = {'A', 'B', 'C', 'D'};

  rc = sb_append_n(&sb, raw, sizeof raw);
  expect(rc == 0, "append_n raw");
  expect(sb_invariant_ok(&sb), "invariant after append_n");
  // 文字列としては途中にNULがあるので strcmp は不可、lenで検査
  expect(sb.len == strlen("Hello, World!") + sizeof raw, "len after append_n");

  // 4) appendf (snprintf 2-pass)
  rc = sb_appendf(&sb, " [%s:%d]", "port", 443);
  expect(rc == 0, "appendf");
  expect(sb_invariant_ok(&sb), "invariant after appendf");
  expect(strstr(sb.p, "[port:443]") != NULL, "appendf content");

  // 5) detach (move)
  char *owned = sb_detach(&sb);
  expect(owned != NULL, "detach not null");
  expect(sb.p == NULL && sb.len == 0 && sb.cap == 0, "sb emptied after detach");
  // owned は free すべき
  expect(strstr(owned, "Hello, World!") != NULL, "owned contains base content");
  free(owned);

  // 6) destroy should be safe on empty
  sb_destroy(&sb);
  expect(sb.p == NULL && sb.len == 0 && sb.cap == 0, "destroy empty ok");

  puts("OK");
  return 0;
}
