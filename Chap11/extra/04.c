#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int load_all(const char *path, unsigned char **out_buf, size_t *out_len);

static void use_hex_dump(const unsigned char *buf, size_t len) {
  size_t n = len < 64 ? len : 64; // 先頭64バイトだけ表示
  for (size_t i = 0; i < n; i++) {
    printf("%02X%s", buf[i], ((i + 1) % 16 == 0 || i + 1 == n) ? "\n" : " ");
  }
  if (len > n) {
    printf("... (%zu bytes total)\n", len);
  }
}

static bool is_png(const unsigned char *buf, size_t len) {
  static const unsigned char sig[8] = {0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A};
  return len >= 8 && memcmp(buf, sig, 8) == 0;
}

static uint64_t fnv1a64(const unsigned char *buf, size_t len) {
  uint64_t h = 1469598103934665603ULL;
  for (size_t i = 0; i < len; i++) {
    h ^= (uint64_t)buf[i];
    h *= 1099511628211ULL;
  }
  return h;
}

int main(void) {
  unsigned char *buf = NULL;
  size_t len = 0;

  int rc = load_all("data/Chap11/input.bin", &buf, &len);
  if (rc != 0) {
    fprintf(stderr, "load_all failed: %s\n", strerror(rc));
    return 1;
  }

  // use例
  use_hex_dump(buf, len);
  printf("is_png=%s\n", is_png(buf, len) ? "true" : "false");
  printf("fnv1a64=%016llX\n", (unsigned long long)fnv1a64(buf, len));

  free(buf);
  return 0;
}

int load_all(const char *path, unsigned char **out_buf, size_t *out_len) {
  if (!out_buf || !out_len) {
    return EINVAL;
  }
  *out_buf = NULL;
  *out_len = 0;
  if (!path) {
    return EINVAL;
  }
  FILE *fp = fopen(path, "rb");
  if (!fp) {
    return errno;
  }

  int rc = 0;
  unsigned char *buf = NULL;

  if (fseek(fp, 0, SEEK_END) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  long pos = ftell(fp);
  if (pos < 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  if (fseek(fp, 0, SEEK_SET) != 0) {
    rc = errno ? errno : EIO;
    goto cleanup;
  }

  size_t len = (size_t)pos;
  buf = (len == 0) ? malloc(1) : malloc(len);
  if (!buf) {
    rc = ENOMEM;
    goto cleanup;
  }

  size_t nread = 0;
  if (len > 0) {
    nread = fread(buf, 1, len, fp);
    if (nread != len) {
      rc = ferror(fp) ? (errno ? errno : EIO) : ENODATA;
      goto cleanup;
    }
  }
  *out_buf = buf;
  *out_len = len;
  buf = NULL; // 所有権移譲
  rc = 0;

cleanup:
  if (fp) {
    fclose(fp);
  }
  free(buf);
  return rc;
}
