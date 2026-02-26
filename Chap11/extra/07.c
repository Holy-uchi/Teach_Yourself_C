#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *p;
  size_t n;
} Blob;

int blob_from_str(const char *s, Blob *out);
void blob_destroy(Blob *b);

int main(void) {
  Blob blob = {0};
  int rc = 0;

  rc = blob_from_str("Hello, World!", &blob);
  if (rc != 0) {
    goto cleanup;
  }

  // blobを使用
  fwrite(blob.p, 1, blob.n, stdout);
  fputc('\n', stdout);

cleanup:
  blob_destroy(&blob);
  return rc;
}

int blob_from_str(const char *s, Blob *out) {
  int rc = 0;
  char *buf = NULL;
  if (!s || !out || out->p != NULL || out->n != 0) {
    rc = EINVAL;
    goto cleanup;
  }

  size_t len = strlen(s);
  buf = malloc(len + 1);
  if (!buf) {
    rc = ENOMEM;
    goto cleanup;
  }
  memcpy(buf, s, len + 1); // NULL終端まで

  // 確保成功
  out->p = buf;
  out->n = len;
  buf = NULL;
cleanup:
  free(buf);
  return rc;
}

void blob_destroy(Blob *b) {
  if (!b)
    return;
  free(b->p);
  b->p = NULL;
  b->n = 0;
}

void blob_free(Blob *b) {
  if (!b)
    return;
  blob_destroy(b);
  free(b);
}
