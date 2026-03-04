#include "load-file.h"
#include "read-line.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int load_file(const char *path, LineVec *lv) {
  if (!path || !lv) {
    errno = EINVAL;
    return -1;
  }
  if (lv->p || lv->len || lv->cap) {
    errno = EINVAL;
    return -1;
  }
  int rc = 0;
  LineVec lv_local = {0};
  FILE *fp = NULL;
  char *line = NULL;
  lv_init(&lv_local);

  fp = fopen(path, "rb");
  if (!fp) {
    if (!errno) {
      errno = EIO;
    }
    rc = -1;
    goto cleanup;
  }

  errno = 0;
  for (;;) {
    rc = read_line(fp, &line);
    if (rc == 1) {
      int rc2 = lv_push(&lv_local, line);
      if (rc2) {
        errno = rc2;
        rc = -1;
        goto cleanup;
      }
      line = NULL; // 念の為NULL化しておく
      continue;
    } else if (rc == 0) {
      // 読み終わり
      break;
    } else {
      // エラー
      if (!errno) {
        errno = EIO;
      }
      rc = -1;
      goto cleanup;
    }
  }
  // 成功時はlv_localの参照を削除する。
  *lv = lv_local;
  lv_init(&lv_local);

  // 成功時は必ずline=NULLにすること
cleanup:
  if (fp) {
    fclose(fp);
  }
  if (line) {
    free(line);
  }

  lv_destroy(&lv_local);

  return rc;
}
