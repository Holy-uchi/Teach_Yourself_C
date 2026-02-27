// build: cc -std=c11 -Wall -Wextra -pedantic -O0 -g drill_a_read_line.c && ./a.out
// asan : cc -std=c11 -Wall -Wextra -pedantic -O0 -g -fsanitize=address,undefined
// drill_a_read_line.c && ./a.out

#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * TODO（あなたの担当）
 *
 * int read_line_owned(FILE *fp, char **out);
 *
 * 契約：
 * - 成功：return 0, *out は malloc されたNUL終端文字列（呼び出し側が free）
 * - 失敗：return errno系, *out は NULL のまま（NULL保証）
 *
 * EOFの扱い：
 * - まだ何も読んでいない状態で EOF：return ENODATA（*out は NULL）
 * - 途中まで読んで EOF：それまでの内容を成功として返す（return 0）
 *
 * 改行の扱い：
 * - 入力の末尾 '\n' は「含めない」（行テキストのみ返す）
 *
 * 制約：
 * - fgets固定長は禁止（任意長対応）。fgetcループ + grow が想定。
 * - realloc は一時変数で受けること。
 */
int safe_mul_size(size_t a, size_t b, size_t *out) {
  if (!out) {
    return EINVAL;
  }
  if (a != 0 && b > SIZE_MAX / a) {
    return EOVERFLOW;
  }
  *out = a * b;
  return 0;
}

#define INITIAL_CAP 8
int read_line_owned(FILE *fp, char **out) {
  if (!fp || !out || *out != NULL) {
    return EINVAL;
  }
  int rc = 0;
  int ch = EOF;
  size_t last = 0;
  size_t cap = INITIAL_CAP;
  char *buf = malloc(INITIAL_CAP);
  if (!buf) {
    rc = ENOMEM;
    goto cleanup;
  }
  while ((ch = fgetc(fp)) != EOF) {
    if (ch == '\n') {
      break;
    }
    if (last + 1 >= cap) {
      size_t ncap;
      rc = safe_mul_size(cap, 2, &ncap);
      if (rc) {
        goto cleanup;
      }
      char *tmp = realloc(buf, ncap);
      if (!tmp) {
        rc = ENOMEM;
        goto cleanup;
      }
      buf = tmp;
      tmp = NULL;
      cap = ncap;
    }

    buf[last++] = (char)ch;
  }
  if (ferror(fp)) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }
  if (last == 0 && ch == EOF) {
    rc = ENODATA;
    goto cleanup;
  }
  buf[last] = '\0';
  *out = buf;
  buf = NULL;

cleanup:
  free(buf);
  return rc;
}

/************ ここから下は所与（触らない） ************/
static void expect(int cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "TEST FAIL: %s\n", msg);
    abort();
  }
}

static FILE *make_fp_from_str(const char *s) {
#if defined(__linux__) || defined(__APPLE__)
  return fmemopen((void *)s, strlen(s), "r");
#else
  // 互換用（環境依存）。ここは所与として扱う。
  (void)s;
  return NULL;
#endif
}

int main(void) {
  // 1) 普通の2行（\nあり）
  {
    FILE *fp = make_fp_from_str("abc\ndef\n");
    expect(fp != NULL, "fmemopen available");

    char *line = NULL;
    int rc = read_line_owned(fp, &line);
    expect(rc == 0, "read line1");
    expect(line && strcmp(line, "abc") == 0, "line1 content");
    free(line);

    line = NULL;
    rc = read_line_owned(fp, &line);
    expect(rc == 0, "read line2");
    expect(line && strcmp(line, "def") == 0, "line2 content");
    free(line);

    line = NULL;
    rc = read_line_owned(fp, &line);
    expect(rc == ENODATA, "EOF -> ENODATA");
    expect(line == NULL, "out must be NULL on ENODATA");
    fclose(fp);
  }

  // 2) 最終行に \n が無い（EOFで確定）
  {
    FILE *fp = make_fp_from_str("lastline");
    char *line = NULL;
    int rc = read_line_owned(fp, &line);
    expect(rc == 0, "read lastline");
    expect(line && strcmp(line, "lastline") == 0, "lastline content");
    free(line);

    line = NULL;
    rc = read_line_owned(fp, &line);
    expect(rc == ENODATA, "EOF after lastline");
    fclose(fp);
  }

  // 3) 空行（"\n"）は "" を返す
  {
    FILE *fp = make_fp_from_str("\nX\n");
    char *line = NULL;
    int rc = read_line_owned(fp, &line);
    expect(rc == 0, "read empty line");
    expect(line && strcmp(line, "") == 0, "empty line content");
    free(line);

    line = NULL;
    rc = read_line_owned(fp, &line);
    expect(rc == 0, "read X");
    expect(line && strcmp(line, "X") == 0, "X content");
    free(line);

    fclose(fp);
  }

  // 4) 長い行（grow確認）
  {
    char big[5000];
    for (size_t i = 0; i < sizeof(big) - 2; i++)
      big[i] = 'A';
    big[sizeof(big) - 2] = '\n';
    big[sizeof(big) - 1] = '\0';

    FILE *fp = make_fp_from_str(big);
    char *line = NULL;
    int rc = read_line_owned(fp, &line);
    expect(rc == 0, "read big");
    expect(line != NULL, "big not null");
    expect(strlen(line) == sizeof(big) - 2, "big length");
    free(line);
    fclose(fp);
  }

  puts("OK");
  return 0;
}
