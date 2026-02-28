// drill_d2_command_parse.c
// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g drill_d2_command_parse.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined
// drill_d2_command_parse.c && ./a.out

#include <errno.h>
#include <limits.h>
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

typedef enum {
  CMD_P,    // print
  CMD_I,    // insert text
  CMD_A,    // append text
  CMD_D,    // delete
  CMD_G,    // goto line number
  CMD_W,    // write (optional path)
  CMD_Q,    // quit (refuse if dirty)
  CMD_QBANG // quit! (force)
} CmdKind;

typedef struct {
  CmdKind kind;
  int has_num;
  long num;   // for g <n>
  char *text; // for i/a (owned), or w <path> (owned optional)
} Command;

static void cmd_destroy(Command *c) {
  if (!c)
    return;
  free(c->text);
  c->text = NULL;
}

/* 所与：空白判定 */
static int is_space(char ch) { return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'; }

/* 所与：左右空白をトリムした範囲 [*b,*e) を返す */
static void trim_span(const char *s, const char **b, const char **e) {
  const char *x = s;
  while (*x && is_space(*x))
    x++;
  const char *y = s + strlen(s);
  while (y > x && is_space(y[-1]))
    y--;
  *b = x;
  *e = y;
}
/************ ここまで所与 ************/

/*
 * TODO（あなたの担当）
 *
 * int parse_command(const char* line, Command* out)
 *
 * 入力例：
 *   "p"
 *   "d"
 *   "g 12"
 *   "i hello world"
 *   "a   xyz"
 *   "w"
 *   "w  /tmp/note.txt"
 *   "q"
 *   "q!"
 *
 * 契約：
 * - 引数不正: EINVAL
 * - 空行/空白のみ: EINVAL
 * - 未知コマンド: EINVAL
 * - g の数値が不正/範囲外/負: ERANGE
 * - i/a はテキスト必須（空なら EINVAL）
 * - w はテキスト任意（無くてもOK）
 * - out->text は必要な場合のみ malloc した owned を入れる（cmd_destroyがfree）
 * - 失敗時：out は安全なゼロ状態に戻す（text=NULLなど）
 *
 * ヒント：
 * - trim_span で全体の空白を落とす
 * - 先頭1文字で分岐、"q!" は2文字目を見る
 * - 数値は strtol（errno=0→呼ぶ→endptr確認）
 */

int parse_command(const char *line, Command *out) {
  (void)line;
  (void)out;
  return ENOSYS;
}

/************ テスト（触らない） ************/
static void expect_kind(const Command *c, CmdKind k) { expect(c->kind == k, "kind match"); }

int main(void) {
  Command c;

  // p
  memset(&c, 0, sizeof(c));
  expect(parse_command(" p ", &c) == 0, "parse p");
  expect_kind(&c, CMD_P);
  cmd_destroy(&c);

  // q!
  memset(&c, 0, sizeof(c));
  expect(parse_command("q!", &c) == 0, "parse q!");
  expect_kind(&c, CMD_QBANG);
  cmd_destroy(&c);

  // g 12
  memset(&c, 0, sizeof(c));
  expect(parse_command("g 12", &c) == 0, "parse g 12");
  expect_kind(&c, CMD_G);
  expect(c.has_num == 1 && c.num == 12, "g num ok");
  cmd_destroy(&c);

  // i text
  memset(&c, 0, sizeof(c));
  expect(parse_command("i hello world", &c) == 0, "parse i text");
  expect_kind(&c, CMD_I);
  expect(c.text && strcmp(c.text, "hello world") == 0, "i text ok");
  cmd_destroy(&c);

  // w optional path
  memset(&c, 0, sizeof(c));
  expect(parse_command("w /tmp/note.txt", &c) == 0, "parse w path");
  expect_kind(&c, CMD_W);
  expect(c.text && strcmp(c.text, "/tmp/note.txt") == 0, "w path ok");
  cmd_destroy(&c);

  // w no arg
  memset(&c, 0, sizeof(c));
  expect(parse_command("w", &c) == 0, "parse w");
  expect_kind(&c, CMD_W);
  expect(c.text == NULL, "w no text");
  cmd_destroy(&c);

  // errors
  memset(&c, 0, sizeof(c));
  expect(parse_command("   ", &c) == EINVAL, "blank => EINVAL");

  memset(&c, 0, sizeof(c));
  expect(parse_command("x", &c) == EINVAL, "unknown => EINVAL");

  memset(&c, 0, sizeof(c));
  expect(parse_command("g -1", &c) == ERANGE, "g negative => ERANGE");

  memset(&c, 0, sizeof(c));
  expect(parse_command("i   ", &c) == EINVAL, "i needs text");

  puts("OK");
  return 0;
}
