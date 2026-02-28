// drill_c_atomic_save.c
// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g drill_c_atomic_save.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined
// drill_c_atomic_save.c && ./a.out
//
// macOS/Linux 想定（POSIX: mkdir/unlink/rename/mkdtemp）
// Windows は対象外

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/errno.h>
#include <sys/stat.h>
#include <unistd.h> // mkdtemp, unlink, rmdir

typedef struct {
  char **p;   // 各要素は「行のowned文字列」（改行なし）
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

  size_t needed = 0;
  int rc = safe_add_size(v->len, 1, &needed);
  if (rc)
    return rc;

  rc = lv_reserve(v, needed);
  if (rc)
    return rc;

  v->p[v->len] = owned_line; // move
  v->len++;
  return 0;
}

/* ファイルを全部読んで文字列として返す（テスト用） */
static int read_all_text(const char *path, char **out_text) {
  if (!path || !out_text)
    return EINVAL;
  *out_text = NULL;

  FILE *fp = fopen(path, "rb");
  if (!fp)
    return errno ? errno : EIO;

  int rc = 0;
  char *buf = NULL;

  if (fseek(fp, 0, SEEK_END) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }
  long size = ftell(fp);
  if (size < 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }
  if (fseek(fp, 0, SEEK_SET) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  size_t n = (size_t)size;
  buf = (char *)malloc(n + 1);
  if (!buf) {
    rc = ENOMEM;
    goto cleanup;
  }

  size_t got = fread(buf, 1, n, fp);
  if (got != n) {
    if (ferror(fp))
      rc = errno ? errno : EIO;
    else
      rc = EIO;
    goto cleanup;
  }
  buf[n] = '\0';

cleanup:
  if (fp) {
    if (fclose(fp) != 0 && rc == 0)
      rc = errno ? errno : EIO;
  }
  if (rc != 0) {
    free(buf);
    buf = NULL;
  }
  *out_text = buf;
  return rc;
}

/* テスト用：期待するテキストと一致するか */
static void expect_file_text_eq(const char *path, const char *expect_text) {
  char *got = NULL;
  int rc = read_all_text(path, &got);
  expect(rc == 0, "read_all_text ok");
  expect(strcmp(got, expect_text) == 0, "file content match");
  free(got);
}

static int write_text_direct(const char *path, const char *text) {
  if (!path || !text)
    return EINVAL;
  FILE *fp = fopen(path, "wb");
  if (!fp)
    return errno ? errno : EIO;

  int rc = 0;
  size_t n = strlen(text);
  if (fwrite(text, 1, n, fp) != n) {
    rc = errno ? errno : EIO;
  }
  if (rc == 0 && fflush(fp) != 0) {
    rc = errno ? errno : EIO;
  }
  if (fclose(fp) != 0 && rc == 0) {
    rc = errno ? errno : EIO;
  }
  return rc;
}
/************ ここまで所与 ************/

/*
 * TODO（あなたの担当）
 *
 * write_all_text_atomic()
 *
 * 目的：
 * - v の全行を path に保存する
 * - 途中で失敗しても「元の path を壊さない」（truncateしない）
 *   -> tmp に全量書いてから rename する
 *
 * 契約：
 * - 引数不正: EINVAL
 * - 失敗時: errno を基本に返す（errno==0なら EIO などでOK）
 * - 失敗時に path が存在していたなら、その中身は保存前と同じであること
 * - v->p[i] は「改行なし」の行文字列。保存時に各行の末尾へ '\n' を付けて書く
 * - tmp 名は「path + ".tmp"」でよい（同一ディレクトリに作る）
 *
 * 重要ポイント：
 * - fopen(tmp,"wb") 失敗
 * - fwrite / fputc 失敗（ferror(fp) / errno）
 * - fflush 失敗
 * - fclose 失敗（書き込み時は珍しくない）
 * - rename(tmp, path) 失敗
 *
 * 片付け：
 * - 失敗したら tmp を unlink できるならする（unlink失敗は主エラーを上書きしない）
 *
 * 実装ヒント：
 * - tmp_path を malloc で組み立てる（サイズ計算は safe_add_size）
 * - rc 単一路線 + goto cleanup が書きやすい
 */

static int make_tmp_path(const char *path, char **out_tmp_path) {
  if (!path || !out_tmp_path)
    return EINVAL;
  *out_tmp_path = NULL;

  size_t size = 0;
  int rc = 0;
  char *tmp_path = NULL;
  rc = safe_add_size(strlen(path), strlen(".tmp"), &size);
  if (rc) {
    goto cleanup;
  }
  rc = safe_add_size(size, 1, &size);
  if (rc) {
    goto cleanup;
  }
  tmp_path = malloc(size);
  if (!tmp_path) {
    rc = ENOMEM;
    goto cleanup;
  }
  int n = snprintf(tmp_path, size, "%s.tmp", path);
  if ((size_t)(n + 1) != size) { // '\0' の+1
    rc = EIO;
    goto cleanup;
  }

  *out_tmp_path = tmp_path;
  tmp_path = NULL;

  // TODO: path + ".tmp" を安全に作る
  // - size = strlen(path) + strlen(".tmp") + 1
  // - malloc して snprintf / memcpy で作る
  // - 成功で *out_tmp_path に入れる（呼び出し側が free）
cleanup:
  free(tmp_path);
  return rc;
}

int write_all_text_atomic(const char *path, const LineVec *v) {
  int rc = 0;
  FILE *fp = NULL;
  char *tmp_path_owned = NULL;
  if (!path || !v) {
    rc = EINVAL;
    goto cleanup;
  }
  rc = make_tmp_path(path, &tmp_path_owned);
  if (rc) {
    goto cleanup;
  }
  fp = fopen(tmp_path_owned, "wb");
  if (!fp) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  for (size_t i = 0; i < v->len; i++) {
    if (fputs(v->p[i], fp) == EOF) {
      rc = errno ? errno : EIO;
      goto cleanup;
    }
    if (fputc('\n', fp) == EOF) {
      rc = errno ? errno : EIO;
      goto cleanup;
    }
  }

  if (fflush(fp) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  int fd = fileno(fp);
  if (fd < 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  if (fsync(fd) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  if (fclose(fp) != 0) {
    fp = NULL;
    rc = errno ? errno : EIO;
    goto cleanup;
  }
  fp = NULL;

  if (rename(tmp_path_owned, path) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

cleanup:

  if (fp) {
    if (fclose(fp) != 0 && rc == 0) {
      rc = errno ? errno : EIO;
    }
  }
  if (tmp_path_owned) {
    if (rc != 0) {
      // tmpファイルを削除
      (void)unlink(tmp_path_owned);
    }
    // 名前を開放
    free(tmp_path_owned);
  }

  return rc;
}

/************ テスト（触らない） ************/
static void fill_linevec(LineVec *v, const char **lines, size_t n) {
  for (size_t i = 0; i < n; i++) {
    int rc = lv_push(v, dup_cstr(lines[i]));
    expect(rc == 0, "lv_push ok");
  }
}

static void make_temp_dir(char *tmpl /* writable */) {
  char *p = mkdtemp(tmpl);
  expect(p != NULL, "mkdtemp ok");
}

static void join_path(char *out, size_t out_cap, const char *dir, const char *name) {
  int n = snprintf(out, out_cap, "%s/%s", dir, name);
  expect(n > 0 && (size_t)n < out_cap, "snprintf join_path ok");
}

int main(void) {
  // temp directory
  char dir_tmpl[] = "/tmp/drill_atomic_save_XXXXXX";
  make_temp_dir(dir_tmpl);

  char path[PATH_MAX];
  char bad_dir_path[PATH_MAX];
  join_path(path, sizeof(path), dir_tmpl, "note.txt");
  join_path(bad_dir_path, sizeof(bad_dir_path), dir_tmpl, "no_such_dir/note.txt");

  // 事前に元ファイルを作っておく（壊れないことを確認するため）
  {
    int rc = write_text_direct(path, "OLD1\nOLD2\n");
    expect(rc == 0, "write_text_direct old ok");
    expect_file_text_eq(path, "OLD1\nOLD2\n");
  }

  // v = {"A","B","C"} を保存したい
  LineVec v;
  lv_init(&v);
  {
    const char *lines[] = {"A", "B", "C"};
    fill_linevec(&v, lines, 3);
  }

  // 1) 成功ケース：path が v の内容で置き換わる
  {
    int rc = write_all_text_atomic(path, &v);
    expect(rc == 0, "write_all_text_atomic success");
    expect_file_text_eq(path, "A\nB\nC\n");

    // tmp が残ってない（残ってたら掃除できてない可能性）
    char tmp_path[PATH_MAX];
    int n = snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path);
    expect(n > 0 && (size_t)n < sizeof(tmp_path), "tmp snprintf ok");
    // tmp は無いことを期待（存在してたら read できてしまう可能性がある）
    FILE *fp = fopen(tmp_path, "rb");
    expect(fp == NULL, "tmp should not exist");
  }

  // 2) 失敗ケース：存在しないディレクトリへ保存 → 失敗し、元ファイルは壊れない
  {
    // まず元を別内容に戻す
    int rc0 = write_text_direct(path, "BASE\n");
    expect(rc0 == 0, "reset base ok");
    expect_file_text_eq(path, "BASE\n");

    int rc = write_all_text_atomic(bad_dir_path, &v);
    expect(rc != 0, "write_all_text_atomic should fail for bad dir");

    // 「元ファイル」が壊れていないこと（bad_dir_path ではなく path）
    expect_file_text_eq(path, "BASE\n");
  }

  // 3) 引数不正
  {
    int rc = write_all_text_atomic(NULL, &v);
    expect(rc == EINVAL, "NULL path => EINVAL");
    rc = write_all_text_atomic(path, NULL);
    expect(rc == EINVAL, "NULL v => EINVAL");
  }

  lv_destroy(&v);

  // cleanup (best effort)
  unlink(path);
  rmdir(dir_tmpl);

  puts("OK");
  return 0;
}
