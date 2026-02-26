#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int append_str(char **io_buf, size_t *io_cap, const char *s);

int main(void) {

  char *buf = NULL;
  size_t cap = 0;
  int rc = 0;

  rc = append_str(&buf, &cap, "Hello ");
  if (rc != 0) {
    goto cleanup;
  }

  rc = append_str(&buf, &cap, "World!");
  if (rc != 0) {
    goto cleanup;
  }

  puts(buf);
  rc = 0;

cleanup:
  free(buf);
  return rc;
}

#define INITIAL_CAP 16

int append_str(char **io_buf, size_t *io_cap, const char *s) {
  // 1) 引数チェック（契約）
  // - io_buf / io_cap / s がNULLならEINVAL
  if (!io_buf || !io_cap || !s) {
    return EINVAL;
  }
  // - *io_buf がNULLなら *io_cap は0扱い（矛盾してても0に寄せるかEINVALにするか決める）
  if (*io_buf == NULL && *io_cap != 0) {
    return EINVAL;
  }

  // 2) 現在の長さを求める
  // - cur_len: *io_buf がNULLなら0、そうでなければ strlen(*io_buf)
  // - add_len: strlen(s)
  size_t cur_len = (*io_buf == NULL) ? 0 : strlen(*io_buf);
  size_t add_len = strlen(s);

  // 3) 必要サイズ needed を計算
  // - needed = cur_len + add_len + 1  // +1 は '\0'
  // - size_t オーバーフローのガードを入れる（必須：トレーニングポイント）
  if (add_len > SIZE_MAX - cur_len - 1) {
    return EOVERFLOW;
  }
  size_t needed = cur_len + add_len + 1;

  // 4) 容量が足りなければ拡張
  // - new_cap は「倍々」か「max(needed, cap*2, INITIAL_CAP)」みたいに
  // - reallocは必ず一時変数で受ける（失敗時に元を失わないため）
  //   char *nbuf = realloc(*io_buf, new_cap);
  //   if (!nbuf) return ENOMEM;
  // - 成功したらここで初めて *io_buf / *io_cap を更新
  if (needed > *io_cap) {
    char *nbuf;
    size_t new_cap = *io_cap ? *io_cap : INITIAL_CAP;
    while (new_cap < needed) {
      new_cap *= 2;
    }
    nbuf = realloc(*io_buf, new_cap);
    if (!nbuf) {
      return ENOMEM;
    }
    *io_buf = nbuf;
    if (*io_cap > SIZE_MAX / 2) {
      return EOVERFLOW;
    }
    *io_cap = new_cap;
    nbuf = NULL;
  }

  // 5) 追加コピー
  // - memcpy(*io_buf + cur_len, s, add_len);
  // - (*io_buf)[cur_len + add_len] = '\0';
  memcpy(*io_buf + cur_len, s, add_len);
  (*io_buf)[cur_len + add_len] = '\0';

  // 6) return 0
  return 0;
}
