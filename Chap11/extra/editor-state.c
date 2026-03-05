// drill_d3_editor_apply.c
// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g drill_d3_editor_apply.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined
// drill_d3_editor_apply.c && ./a.out

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************ 所与（触らない） ************/
static void expect(int cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "TEST FAIL: %s\n", msg);
    abort();
  }
}

static char *dup_cstr(const char *s) {
  size_t n = strlen(s) + 1;
  char *p = (char *)malloc(n);
  if (!p)
    return NULL;
  memcpy(p, s, n);
  return p;
}

typedef struct {
  char **p;
  size_t len;
  size_t cap;
} LineVec;

static void lv_init(LineVec *v) {
  if (!v)
    return;
  v->p = NULL;
  v->len = 0;
  v->cap = 0;
}

static void lv_destroy(LineVec *v) {
  if (!v)
    return;
  for (size_t i = 0; i < v->len; i++)
    free(v->p[i]);
  free(v->p);
  v->p = NULL;
  v->len = 0;
  v->cap = 0;
}

static int lv_reserve(LineVec *v, size_t min_cap) {
  if (!v)
    return EINVAL;
  if (v->cap >= min_cap)
    return 0;
  size_t ncap = v->cap ? v->cap : 8;
  while (ncap < min_cap)
    ncap *= 2;
  char **tmp = (char **)realloc(v->p, ncap * sizeof(char *));
  if (!tmp)
    return ENOMEM;
  v->p = tmp;
  v->cap = ncap;
  return 0;
}

// move push
static int lv_push(LineVec *v, char *owned_line) {
  if (!v || !owned_line)
    return EINVAL;
  int rc = lv_reserve(v, v->len + 1);
  if (rc)
    return rc;
  v->p[v->len++] = owned_line;
  return 0;
}

// move insert at i (0..len)
// static int lv_insert(LineVec *v, size_t i, char *owned_line) {
//   if (!v || !owned_line)
//     return EINVAL;
//   if (i > v->len)
//     return ERANGE;
//   int rc = lv_reserve(v, v->len + 1);
//   if (rc)
//     return rc;
//   memmove(&v->p[i + 1], &v->p[i], (v->len - i) * sizeof(v->p[0]));
//   v->p[i] = owned_line;
//   v->len++;
//   return 0;
// }

// static int lv_delete(LineVec *v, size_t i) {
//   if (!v)
//     return EINVAL;
//   if (i >= v->len)
//     return ERANGE;
//   free(v->p[i]);
//   size_t tail = v->len - i - 1;
//   if (tail)
//     memmove(&v->p[i], &v->p[i + 1], tail * sizeof(v->p[0]));
//   v->len--;
//   return 0;
// }

static void assert_lines(const LineVec *v, const char **e, size_t n) {
  expect(v->len == n, "len match");
  for (size_t i = 0; i < n; i++)
    expect(strcmp(v->p[i], e[i]) == 0, "line match");
}

typedef enum { CMD_I, CMD_A, CMD_D, CMD_G } CmdKind;

typedef struct {
  CmdKind kind;
  long num;   // for G (1-based line number)
  char *text; // for I/A (owned by command)
} Command;

typedef struct {
  LineVec lines;
  size_t cur; // 0..len (len==0 then cur==0)
  int dirty;  // bool
} Editor;

static void ed_init(Editor *e) {
  lv_init(&e->lines);
  e->cur = 0;
  e->dirty = 0;
}

static void ed_destroy(Editor *e) { lv_destroy(&e->lines); }

static void ed_seed(Editor *e, const char **lines, size_t n) {
  for (size_t i = 0; i < n; i++)
    expect(lv_push(&e->lines, dup_cstr(lines[i])) == 0, "seed push");
  e->cur = (n == 0) ? 0 : 0;
  e->dirty = 0;
}

static void ed_snapshot(const Editor *src, Editor *dst) {
  // deep copy for test comparison
  ed_init(dst);
  for (size_t i = 0; i < src->lines.len; i++)
    expect(lv_push(&dst->lines, dup_cstr(src->lines.p[i])) == 0, "snap");
  dst->cur = src->cur;
  dst->dirty = src->dirty;
}

static int ed_equal(const Editor *a, const Editor *b) {
  if (a->cur != b->cur || a->dirty != b->dirty)
    return 0;
  if (a->lines.len != b->lines.len)
    return 0;
  for (size_t i = 0; i < a->lines.len; i++)
    if (strcmp(a->lines.p[i], b->lines.p[i]) != 0)
      return 0;
  return 1;
}
/************ ここまで所与 ************/

/*
 * TODO（あなたの担当）
 *
 * int editor_apply(Editor* e, const Command* c)
 *
 * 仕様（ここが“状態遷移テスト”）：
 * - e/c NULL: EINVAL
 *
 * CMD_G: 1-based 行番号へ移動
 *   - c->num が 1..len 以外なら ERANGE
 *   - 成功: cur = (num-1), dirtyは変えない
 *
 * CMD_I: 現在行の「前」に text を挿入（text必須）
 *   - len==0 の場合は 0 に挿入
 *   - 成功: cur は挿入した行を指す（=挿入位置）, dirty=1
 *
 * CMD_A: 現在行の「後」に text を挿入（text必須）
 *   - len==0 の場合は 0 に挿入（結果 cur=0）
 *   - 成功: cur は挿入した行を指す, dirty=1
 *
 * CMD_D: 現在行を削除
 *   - len==0 は ERANGE
 *   - 成功:
 *     - len が 0 になったら cur=0
 *     - それ以外で、cur が末尾を超えたら cur=len-1（末尾に寄せる）
 *     - dirty=1
 *
 * 重要：失敗時は状態不変（lines/cur/dirty全部）
 *
 * ヒント：
 * - 失敗時不変は「先に検証→最後に変更」か、「コピーして成功時にswap」が簡単
 * - insert は LineVec が move契約なので、挿入用に dup を作って、失敗時 free する
 */

int editor_apply(Editor *e, const Command *c) {
  (void)e;
  (void)c;
  return ENOSYS;
}

/************ テスト（触らない） ************/
int main(void) {
  Editor ed;
  ed_init(&ed);
  const char *seed[] = {"L1", "L2", "L3"};
  ed_seed(&ed, seed, 3);

  // G 2 => cur=1
  {
    Editor before;
    ed_snapshot(&ed, &before);
    Command c = {.kind = CMD_G, .num = 2, .text = NULL};
    int rc = editor_apply(&ed, &c);
    expect(rc == 0, "G ok");
    expect(ed.cur == 1, "cur moved");
    expect(ed.dirty == 0, "dirty unchanged");
    ed_destroy(&before);
  }

  // I "H" at cur=1 => insert before index 1, cur becomes 1 (points to inserted)
  {
    Command c = {.kind = CMD_I, .num = 0, .text = "H"};
    int rc = editor_apply(&ed, &c);
    expect(rc == 0, "I ok");
    const char *e[] = {"L1", "H", "L2", "L3"};
    assert_lines(&ed.lines, e, 4);
    expect(ed.cur == 1, "cur at inserted");
    expect(ed.dirty == 1, "dirty set");
  }

  // A "X" after cur=1 => insert at 2, cur becomes 2
  {
    Command c = {.kind = CMD_A, .num = 0, .text = "X"};
    int rc = editor_apply(&ed, &c);
    expect(rc == 0, "A ok");
    const char *e[] = {"L1", "H", "X", "L2", "L3"};
    assert_lines(&ed.lines, e, 5);
    expect(ed.cur == 2, "cur at appended");
  }

  // D delete cur=2 ("X") => cur stays 2 now points to "L2"
  {
    Command c = {.kind = CMD_D, .num = 0, .text = NULL};
    int rc = editor_apply(&ed, &c);
    expect(rc == 0, "D ok");
    const char *e[] = {"L1", "H", "L2", "L3"};
    assert_lines(&ed.lines, e, 4);
    expect(ed.cur == 2, "cur stable");
  }

  // failure must keep state: G out of range
  {
    Editor before;
    ed_snapshot(&ed, &before);
    Command c = {.kind = CMD_G, .num = 99, .text = NULL};
    int rc = editor_apply(&ed, &c);
    expect(rc == ERANGE, "G out => ERANGE");
    expect(ed_equal(&ed, &before), "state unchanged on fail");
    ed_destroy(&before);
  }

  // delete until empty: cur behavior
  {
    // set cur=0 and delete 4 times
    ed.cur = 0;
    Command d = {.kind = CMD_D, .num = 0, .text = NULL};
    expect(editor_apply(&ed, &d) == 0, "del1");
    expect(editor_apply(&ed, &d) == 0, "del2");
    expect(editor_apply(&ed, &d) == 0, "del3");
    expect(editor_apply(&ed, &d) == 0, "del4");
    expect(ed.lines.len == 0, "empty now");
    expect(ed.cur == 0, "cur=0 when empty");
    expect(editor_apply(&ed, &d) == ERANGE, "delete empty => ERANGE");
  }

  ed_destroy(&ed);
  puts("OK");
  return 0;
}
