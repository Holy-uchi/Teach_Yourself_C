// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g drill_b_linevec.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined drill_b_linevec.c
// && ./a.out

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char **p;   // 各要素は「行のowned文字列」
  size_t len; // 行数
  size_t cap; // 配列容量
} LineVec;

/************ 所与（触らない） ************/
static void lv_init(LineVec *v) {
  if (!v)
    return;
  v->p = NULL;
  v->len = 0;
  v->cap = 0;
}

static void expect(int cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "TEST FAIL: %s\n", msg);
    abort();
  }
}

static char *dup_cstr(const char *s) {
  size_t n = strlen(s) + 1;
  char *p = malloc(n);
  if (!p)
    return NULL;
  memcpy(p, s, n);
  return p;
}
/************ ここまで所与 ************/

/*
 * TODO（あなたの担当）
 *
 * 契約（重要）：
 * - v が所有者：v->p[i] はすべて malloc 文字列。lv_destroy が全部 free する。
 * - insert/push は owned_line の所有権を受け取る（move）
 *   - 成功：vが所有、呼び出し側はもう free しない
 *   - 失敗：vは不変、呼び出し側が owned_line を free すべき（= 受け取れていない）
 *
 * エラー：
 * - 不正引数は EINVAL
 * - 範囲外は ERANGE（例：insertで i > len）
 */

// 全解放（要素free→配列free→{0}）
void lv_destroy(LineVec *v) {
  if (!v) {
    return;
  }
  for (size_t i = 0; i < v->len; i++) {
    free(v->p[i]);
  }
  free(v->p);
  v->p = NULL;
  v->len = 0;
  v->cap = 0;
  return;
}

static inline int safe_mul_size(size_t a, size_t b, size_t *out) {
  if (!out) {
    return EINVAL;
  }
  if (a != 0 && b > SIZE_MAX / a) {
    return EOVERFLOW;
  }
  *out = a * b;
  return 0;
}

static inline int safe_add_size(size_t a, size_t b, size_t *out) {
  if (!out) {
    return EINVAL;
  }
  if (b > SIZE_MAX - a) {
    return EOVERFLOW;
  }
  *out = a + b;
  return 0;
}

// 容量確保：cap >= min_cap
int lv_reserve(LineVec *v, size_t min_cap) {
  if (!v) {
    return EINVAL;
  }
  if (v->cap >= min_cap) {
    return 0;
  }
  int rc;
  size_t ncap = v->cap ? v->cap : 8;
  while (ncap < min_cap) {
    rc = safe_mul_size(ncap, 2, &ncap);
    if (rc) {
      return rc;
    }
  }
  size_t bytes;
  rc = safe_mul_size(ncap, sizeof(char *), &bytes);
  if (rc) {
    return rc;
  }

  char **tmp = realloc(v->p, bytes);
  if (!tmp) {
    return ENOMEM;
  }
  v->p = tmp;
  tmp = NULL;
  v->cap = ncap;
  return 0;
}

// 末尾に追加（move）
int lv_push(LineVec *v, char *owned_line) {
  if (!v || !owned_line) {
    return EINVAL;
  }
  int rc = 0;

  size_t needed = 0;
  rc = safe_add_size(v->len, 1, &needed);
  if (rc) {
    return rc;
  }

  rc = lv_reserve(v, needed);
  if (rc) {
    return rc;
  }

  v->p[v->len] = owned_line;
  v->len++;
  owned_line = NULL;
  // for (size_t i = 0; i < v->len; i++) {
  //   printf("%zu: %s\n", i, v->p[i]);
  // }
  // printf("len: %zu, cap: %zu\n", v->len, v->cap);
  return 0;
}

// i に挿入（0..len）。i==len は末尾挿入（move）
int lv_insert(LineVec *v, size_t i, char *owned_line) {
  if (!v || !owned_line) {
    return EINVAL;
  }
  if (i > v->len) {
    return ERANGE;
  }
  int rc = 0;

  size_t needed = 0;
  rc = safe_add_size(v->len, 1, &needed);
  if (rc) {
    return rc;
  }

  rc = lv_reserve(v, needed);
  if (rc) {
    return rc;
  }

  // 挿入
  memmove(&v->p[i + 1], &v->p[i], (v->len - i) * sizeof(v->p[0]));
  v->p[i] = owned_line;
  v->len++;
  owned_line = NULL;

  return 0;
}

// i を削除（0..len-1）。その行は free される
int lv_delete(LineVec *v, size_t i) {
  if (!v) {
    return EINVAL;
  }
  if (i >= v->len) {
    return ERANGE;
  }

  // 目的の行を削除
  free(v->p[i]);
  // 一個ずつ前に詰める
  size_t tail = v->len - i - 1;
  if (tail > 0) {
    memmove(&v->p[i], &v->p[i + 1], tail * sizeof(v->p[0]));
  }
  // lenをデクリメント
  v->len--;

  return 0;
}

/************ テスト（触らない） ************/
static void assert_lines(LineVec *v, const char **expect_s, size_t n) {
  expect(v->len == n, "len match");
  for (size_t i = 0; i < n; i++) {
    expect(v->p[i] != NULL, "line not null");
    expect(strcmp(v->p[i], expect_s[i]) == 0, "line content match");
  }
}

int main(void) {
  LineVec v;
  lv_init(&v);

  // 1) push 3 lines
  int rc = 0;
  rc = lv_push(&v, dup_cstr("L1"));
  expect(rc == 0, "push L1");
  rc = lv_push(&v, dup_cstr("L2"));
  expect(rc == 0, "push L2");
  rc = lv_push(&v, dup_cstr("L3"));
  expect(rc == 0, "push L3");
  {
    const char *e[] = {"L1", "L2", "L3"};
    assert_lines(&v, e, 3);
  }

  // 2) insert at head
  rc = lv_insert(&v, 0, dup_cstr("H"));
  expect(rc == 0, "insert head");
  {
    const char *e[] = {"H", "L1", "L2", "L3"};
    assert_lines(&v, e, 4);
  }

  // 3) insert in middle
  rc = lv_insert(&v, 2, dup_cstr("M"));
  expect(rc == 0, "insert middle");
  {
    const char *e[] = {"H", "L1", "M", "L2", "L3"};
    assert_lines(&v, e, 5);
  }

  // 4) delete middle
  rc = lv_delete(&v, 2); // delete "M"
  expect(rc == 0, "delete middle");
  {
    const char *e[] = {"H", "L1", "L2", "L3"};
    assert_lines(&v, e, 4);
  }

  // 5) delete head
  rc = lv_delete(&v, 0); // delete "H"
  expect(rc == 0, "delete head");
  {
    const char *e[] = {"L1", "L2", "L3"};
    assert_lines(&v, e, 3);
  }

  // 6) insert at tail (i==len)
  rc = lv_insert(&v, v.len, dup_cstr("T"));
  expect(rc == 0, "insert tail");
  {
    const char *e[] = {"L1", "L2", "L3", "T"};
    assert_lines(&v, e, 4);
  }

  // 7) delete last
  rc = lv_delete(&v, v.len - 1); // delete "T"
  expect(rc == 0, "delete last");
  {
    const char *e[] = {"L1", "L2", "L3"};
    assert_lines(&v, e, 3);
  }

  // 8) error cases (range)
  rc = lv_insert(&v, v.len + 1, dup_cstr("X"));
  expect(rc == ERANGE, "insert out of range");
  rc = lv_delete(&v, v.len); // out of range
  expect(rc == ERANGE, "delete out of range");

  lv_destroy(&v);
  puts("OK");
  return 0;
}
