// drill_d1_load_linevec.c
// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g drill_d1_load_linevec.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined
// drill_d1_load_linevec.c && ./a.out
//
// macOS/Linux 想定（POSIX: mkdtemp, unlink, rmdir）

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/errno.h>
#include <unistd.h>

typedef struct {
  char **p; // owned lines (no trailing '\n')
  size_t len;
  size_t cap;
} LineVec;

/************ 所与（触らない） ************/
static void expect(int cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "TEST FAIL: %s\n", msg);
    abort();
  }
}

//  static char *dup_cstr(const char *s) {
//    size_t n = strlen(s) + 1;
//    char *p = (char *)malloc(n);
//    if (!p)
//      return NULL;
//    memcpy(p, s, n);
//    return p;
//  }

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

static int safe_add_size(size_t a, size_t b, size_t *out) {
  if (!out)
    return EINVAL;
  if (b > SIZE_MAX - a)
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

static int lv_reserve(LineVec *v, size_t min_cap) {
  if (!v)
    return EINVAL;
  if (v->cap >= min_cap)
    return 0;
  size_t ncap = v->cap ? v->cap : 8;
  while (ncap < min_cap) {
    int rc = safe_mul_size(ncap, 2, &ncap);
    if (rc)
      return rc;
  }
  size_t bytes = 0;
  int rc = safe_mul_size(ncap, sizeof(char *), &bytes);
  if (rc)
    return rc;

  char **tmp = (char **)realloc(v->p, bytes);
  if (!tmp)
    return ENOMEM;
  v->p = tmp;
  v->cap = ncap;
  return 0;
}

static int lv_push(LineVec *v, char *owned_line) {
  if (!v || !owned_line)
    return EINVAL;
  size_t need = 0;
  int rc = safe_add_size(v->len, 1, &need);
  if (rc)
    return rc;
  rc = lv_reserve(v, need);
  if (rc)
    return rc;
  v->p[v->len++] = owned_line;
  return 0;
}

/*
 * 所与：read_line_owned(fp, &out_line)
 * - 1行を読み取り、末尾の '\n' は「含めない」形で返す（=行本体）
 * - EOFで何も読めなければ: out_line=NULL を返し、rc=0
 * - エラーなら rc!=0
 *
 * ※実装は「触らない」想定。ここでは簡易版を所与として提供。
 */
static int read_line_owned(FILE *fp, char **out_line) {
  if (!fp || !out_line)
    return EINVAL;
  *out_line = NULL;

  size_t cap = 64;
  size_t len = 0;
  char *buf = (char *)malloc(cap);
  if (!buf)
    return ENOMEM;

  int c;
  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n')
      break;
    if (len + 1 >= cap) {
      size_t ncap = cap * 2;
      char *tmp = (char *)realloc(buf, ncap);
      if (!tmp) {
        free(buf);
        return ENOMEM;
      }
      buf = tmp;
      cap = ncap;
    }
    buf[len++] = (char)c;
  }

  if (c == EOF && len == 0) {
    free(buf);
    return 0;
  } // EOF no data
  buf[len] = '\0';
  *out_line = buf;
  return 0;
}

static int write_text_direct(const char *path, const char *text) {
  FILE *fp = fopen(path, "wb");
  if (!fp)
    return errno ? errno : EIO;
  int rc = 0;
  size_t n = strlen(text);
  if (fwrite(text, 1, n, fp) != n)
    rc = errno ? errno : EIO;
  if (rc == 0 && fflush(fp) != 0)
    rc = errno ? errno : EIO;
  if (fclose(fp) != 0 && rc == 0)
    rc = errno ? errno : EIO;
  return rc;
}

static void assert_lines_eq(const LineVec *v, const char **e, size_t n) {
  expect(v->len == n, "len match");
  for (size_t i = 0; i < n; i++) {
    expect(v->p[i] != NULL, "line not null");
    expect(strcmp(v->p[i], e[i]) == 0, "line content match");
  }
}
/************ ここまで所与 ************/

/*
 * TODO（あなたの担当）
 *
 * int load_file_into_linevec(const char* path, LineVec* out, size_t max_line_bytes)
 *
 * 契約：
 * - 引数不正: EINVAL
 * - ファイルを読み込んで out に行配列として格納（各行は改行なし）
 * - CRLF 対応：行末が '\r' で終わっていたらそれを落とす
 * - 最終行が改行で終わってなくても1行として採用
 * - 1行の長さが max_line_bytes を超えたら EOVERFLOW（または ERANGE でも可）
 * - 失敗時：out は不変（0行のまま or 事前状態を保持）にすること
 *
 * ヒント：
 * - tmp LineVec を作って、成功したら move で out に移すのが簡単
 * - read_line_owned() をループ
 * - '\r' 落としは末尾1文字チェックでOK
 */

int load_file_into_linevec(const char *path, LineVec *out, size_t max_line_bytes) {
  int rc = 0;
  LineVec lv_tmp = {0};
  lv_init(&lv_tmp);
  char *line = NULL;
  FILE *fp = NULL;
  if (!path || !out) {
    rc = EINVAL;
    goto cleanup;
  }

  fp = fopen(path, "rb");
  if (!fp) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  for (;;) {
    line = NULL;
    rc = read_line_owned(fp, &line);
    if (rc != 0) {
      goto cleanup;
    }
    if (line == NULL) {
      break;
    } // EOF
    //
    size_t len = strlen(line);
    if (len > max_line_bytes) {
      rc = EOVERFLOW;
      free(line);
      line = NULL;
      goto cleanup;
    }
    // 正常
    if (len > 0 && line[len - 1] == '\r') {
      // windowsの改行対策
      line[len - 1] = '\0';
    }
    int rc2 = lv_push(&lv_tmp, line);
    if (rc2 != 0) {
      rc = rc2;
      free(line);
      line = NULL;
      goto cleanup;
    }
    line = NULL; // moved;
  }

  *out = lv_tmp;
  lv_tmp.p = NULL;
  lv_tmp.len = 0;
  lv_tmp.cap = 0;
cleanup:
  if (fp) {
    if (fclose(fp) != 0 && rc == 0) {
      rc = errno ? errno : EIO;
    }
  }
  lv_destroy(&lv_tmp);
  free(line); // 念の為
  // lv_tmpの解放は不要。
  return rc;
}

/************ テスト（触らない） ************/
static void make_temp_dir(char *tmpl /* writable */) {
  char *p = mkdtemp(tmpl);
  expect(p != NULL, "mkdtemp ok");
}

static void join_path(char *out, size_t out_cap, const char *dir, const char *name) {
  int n = snprintf(out, out_cap, "%s/%s", dir, name);
  expect(n > 0 && (size_t)n < out_cap, "snprintf join_path ok");
}

int main(void) {
  char dir_tmpl[] = "/tmp/drill_d1_load_XXXXXX";
  make_temp_dir(dir_tmpl);

  char path[PATH_MAX];
  join_path(path, sizeof(path), dir_tmpl, "note.txt");

  // 1) LF
  {
    int rc = write_text_direct(path, "A\nB\nC\n");
    expect(rc == 0, "write lf ok");

    LineVec v;
    lv_init(&v);
    rc = load_file_into_linevec(path, &v, 1024);
    expect(rc == 0, "load lf ok");
    const char *e[] = {"A", "B", "C"};
    assert_lines_eq(&v, e, 3);
    lv_destroy(&v);
  }

  // 2) CRLF
  {
    int rc = write_text_direct(path, "A\r\nB\r\nC\r\n");
    expect(rc == 0, "write crlf ok");

    LineVec v;
    lv_init(&v);
    rc = load_file_into_linevec(path, &v, 1024);
    expect(rc == 0, "load crlf ok");
    const char *e[] = {"A", "B", "C"};
    assert_lines_eq(&v, e, 3);
    lv_destroy(&v);
  }

  // 3) no trailing newline (last line still included)
  {
    int rc = write_text_direct(path, "X\nY\nZ");
    expect(rc == 0, "write no last nl ok");

    LineVec v;
    lv_init(&v);
    rc = load_file_into_linevec(path, &v, 1024);
    expect(rc == 0, "load no last nl ok");
    const char *e[] = {"X", "Y", "Z"};
    assert_lines_eq(&v, e, 3);
    lv_destroy(&v);
  }

  // 4) line too long
  {
    // 11 bytes line without newline
    int rc = write_text_direct(path, "01234567890");
    expect(rc == 0, "write long ok");

    LineVec v;
    lv_init(&v);
    rc = load_file_into_linevec(path, &v, 10); // max 10 => overflow
    expect(rc == EOVERFLOW || rc == ERANGE, "line too long rejected");
    expect(v.len == 0, "out unchanged on failure");
    lv_destroy(&v);
  }

  unlink(path);
  rmdir(dir_tmpl);

  puts("OK");
  return 0;
}
