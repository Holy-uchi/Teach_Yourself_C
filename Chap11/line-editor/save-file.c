#include "save-file.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int make_tmp_path(const char *path, char **out) {
  if (!path || !out) {
    return EINVAL;
  }

  if (*out != NULL) {
    return EINVAL;
  }

  *out = NULL;

  int rc = 0;
  char *tmp_path = NULL;

  size_t len = strlen(path);
  tmp_path = malloc(len + 5); // ".tmp" + '\0' = 5
  if (!tmp_path) {
    rc = ENOMEM;
    goto cleanup;
  }

  rc = snprintf(tmp_path, len + 5, "%s.tmp", path);
  if (rc != (int)len + 4) {
    if (rc == -1 || rc < (int)len + 4) {
      rc = EIO;
    } else {
      // rc > len+4
      // なぜか足りなかった。
      rc = ENOBUFS;
    }
    goto cleanup;
  }

  *out = tmp_path;
  tmp_path = NULL;
  rc = 0;

  // 成功時tmp_pathをNULLにする
cleanup:
  if (tmp_path) {
    free(tmp_path);
  }

  return rc;
}

int save_file(const char *path, LineVec *lv) {
  if (!path || !lv) {
    errno = EINVAL;
    return -1;
  }

  int rc = 0;
  FILE *fp = NULL;
  char *tmp_path = NULL;

  errno = 0;
  int rc2 = make_tmp_path(path, &tmp_path);
  if (rc2) {
    errno = rc2;
    rc = -1;
    goto cleanup;
  }

  fp = fopen(tmp_path, "wb");
  if (!fp) {
    if (!errno) {
      errno = EIO;
    }
    rc = -1;
    goto cleanup;
  }

  for (size_t i = 0; i < lv->len; i++) {
    if (fputs(lv->p[i], fp) == EOF) {
      if (!errno) {
        errno = EIO;
      }
      rc = -1;
      goto cleanup;
    }
    if (fputc('\n', fp) == EOF) {
      if (!errno) {
        errno = EIO;
      }
      rc = -1;
      goto cleanup;
    }
  }

  if (fflush(fp) == EOF) {
    if (!errno) {
      errno = EIO;
    }
    rc = -1;
    goto cleanup;
  }

  if (fclose(fp) == EOF) {
    if (!errno) {
      errno = EIO;
    }
    rc = -1;
    goto cleanup;
  }
  fp = NULL;

  if (rename(tmp_path, path) == -1) {
    // 失敗時はrc = -1 & errnoに理由
    rc = -1;
    goto cleanup;
  }
cleanup:
  if (rc != 0) {
    if (tmp_path) {
      unlink(tmp_path); // 失敗は握りつぶす
    }
  }
  if (fp) {
    fclose(fp);
  }
  if (tmp_path) {
    free(tmp_path);
  }
  return rc;
}
