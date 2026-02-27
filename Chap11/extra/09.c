#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

// 所与：実装済み（呼び出し側は中身を書かない）
int hex_encode(const unsigned char *in, size_t in_len, char *out, size_t out_cap,
               size_t *out_written);

/*
 * TODO（あなたの担当）
 *
 * hex_encode を「2回呼び出し」で安全に使い、
 * malloc した out（呼び出し側所有）を *out_str に返す。
 *
 * 契約：
 * - 成功：return 0, *out_str は malloc されたバッファ（呼び出し側が free）
 * - 失敗：return errno系, *out_str は NULL のまま（汚さない）
 *
 * 注意：
 * - 1回目：小さい out_cap で呼んで ENOBUFS と needed を取る
 * - 2回目：needed で malloc → 再度 hex_encode → 成功させる
 * - ENOBUFS 以外の失敗もあり得る（EINVAL/ENOMEMなど）
 */
//  #define INITIAL_CAP 8
//  int hex_encode_owned(const unsigned char *in, size_t in_len, char **out_str) {
//    // TODO: ここを実装
//    int rc = 0;
//    char *tmp = NULL;
//    if (!out_str || *out_str != NULL) {
//      rc = EINVAL;
//      goto cleanup;
//    }
//    if (!in && in_len != 0) {
//      return EINVAL;
//    }
//    rc = 0;
//    tmp = malloc(INITIAL_CAP);
//    size_t cap = INITIAL_CAP;
//    size_t written = 0;
//    rc = hex_encode(in, in_len, tmp, cap, &written);
//    if (rc != 0 && rc != ENOBUFS) {
//      goto cleanup;
//    } else if (rc == ENOBUFS) {
//      size_t ncap = written;
//      char *ntmp = realloc(tmp, ncap);
//      if (!ntmp) {
//        rc = ENOMEM;
//        goto cleanup;
//      }
//      cap = ncap;
//      tmp = ntmp;
//      ntmp = NULL;
//
//      rc = hex_encode(in, in_len, tmp, cap, &written);
//      if (rc != 0) {
//        goto cleanup;
//      }
//    }
//    *out_str = tmp;
//    tmp = NULL;
//  cleanup:
//    free(tmp);
//    return rc;
//  }
// 模範解答
int hex_encode_owned(const unsigned char *in, size_t in_len, char **out_str) {
  if (!out_str || *out_str != NULL)
    return EINVAL;
  if (!in && in_len != 0)
    return EINVAL;

  int rc = 0;
  char *tmp = NULL;
  size_t needed = 0;

  rc = hex_encode(in, in_len, NULL, 0, &needed);
  if (rc != ENOBUFS) {
    return (rc == 0) ? EINVAL : rc;
  }

  tmp = malloc(needed);
  if (!tmp) {
    return ENOMEM;
  }

  size_t written = 0;
  rc = hex_encode(in, in_len, tmp, needed, &written);
  if (rc != 0) {
    free(tmp);
    return rc;
  }
  if (written > needed) {
    free(tmp);
    return EIO;
  }
  *out_str = tmp;
  return 0;
}

/************ 所与の実装（ここは触らない） ************/
static int nibble_to_hex(unsigned v) { return (v < 10) ? ('0' + (int)v) : ('a' + (int)(v - 10)); }

int hex_encode(const unsigned char *in, size_t in_len, char *out, size_t out_cap,
               size_t *out_written) {
  if (!out_written)
    return EINVAL;
  *out_written = 0;
  if (!in && in_len != 0)
    return EINVAL;

  // 文字列として返す：2*len + 1（NUL）
  if (in_len > (SIZE_MAX - 1) / 2)
    return EOVERFLOW;
  size_t needed = in_len * 2 + 1;

  if (!out || out_cap < needed) {
    *out_written = needed;
    return ENOBUFS;
  }

  for (size_t i = 0; i < in_len; i++) {
    unsigned b = in[i];
    out[i * 2 + 0] = (char)nibble_to_hex((b >> 4) & 0xF);
    out[i * 2 + 1] = (char)nibble_to_hex(b & 0xF);
  }
  out[in_len * 2] = '\0';
  *out_written = needed;
  return 0;
}
/*****************************************************/

int main(void) {
  const unsigned char data[] = {0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x7F};

  char *hex = NULL;
  int rc = hex_encode_owned(data, sizeof data, &hex);
  if (rc != 0) {
    fprintf(stderr, "hex_encode_owned failed: %s\n", strerror(rc));
    return 1;
  }

  printf("hex=%s\n", hex);
  free(hex);

  puts("OK");
  return 0;
}
