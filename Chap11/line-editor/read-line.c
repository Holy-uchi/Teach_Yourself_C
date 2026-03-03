#include "read-line.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 成功: 1, EOF: 0, エラー: -1(errno設定)
int read_line(FILE *fp, char **out) {
  if (!fp || !out || *out != NULL) {
    errno = EINVAL;
    return -1;
  }
  *out = NULL;

  int rc = 1;
  char *tmp = NULL;
  int ch;
  size_t last = 0;
  size_t cap = 0;

  errno = 0;
  while ((ch = fgetc(fp)) != EOF) {
    size_t need = last + 2; // ch + 終端'\0'
    if (need > cap) {
      size_t ncap = cap ? cap : 8;
      while (ncap < need) {
        if (ncap > SIZE_MAX / 2) {
          errno = EOVERFLOW;
          rc = -1;
          goto cleanup;
        }
        ncap *= 2;
      }

      char *ntmp = realloc(tmp, ncap);
      if (!ntmp) {
        errno = ENOMEM;
        rc = -1;
        goto cleanup;
      }
      tmp = ntmp;
      cap = ncap;
    }

    if (ch == '\n') {
      if (last >= 1 && tmp[last - 1] == '\r') {
        // windowsの\r\n
        tmp[last - 1] = '\0';
      }
      break;
    }

    tmp[last] = (char)ch;
    last++;
  }

  if (ch == EOF && last == 0) {
    if (ferror(fp)) {
      if (!errno) {
        errno = EIO;
      }
      rc = -1;
    } else {
      rc = 0;
    }
    goto cleanup;
  }

  tmp[last] = '\0';
  *out = tmp;
  tmp = NULL;

  // ioerror: ok
  // eof: ok
  // windowsの\r\n: ok
  // ^\n
  // enomem

cleanup:
  if (tmp) {
    free(tmp);
  }
  return rc;
}
