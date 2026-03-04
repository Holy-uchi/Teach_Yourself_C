#include "line-editor.h"
#include "save-file.h"
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

int cmd_append(Editor *ed, FILE *in) { return ENOSYS; }
int cmd_replace(Editor *ed, FILE *in) { return ENOSYS; }
int cmd_list(Editor *ed, FILE *out) {
  if (!ed || !out) {
    return EINVAL;
  }

  // 不正パターン
  // 長さが0 => 不正というよりそれを伝えたいが、、、
  if (ed->buf.len == 0) {
    fputs("This file has no lines so far", out);
    return 0;
  }

  size_t start, last;
  start = ed->cur >= 2 ? ed->cur - 2 : 0; // 現在のカーソルより前に2行あるなら cur - 2
  last = (ed->buf.len - (ed->cur + 1)) >= 2
             ? ed->cur + 2
             : ed->buf.len - 1; // 現在のカーソルより後ろに2行以上あるなら cur + 2
}

int cmd_save(Editor *ed) {
  if (!ed) {
    return EINVAL;
  }
  if (!ed->filename) {
    // TODO: stdoutを抽象化すべき？
    // TODO: エラーメッセージは外でやるべき？
    puts("ファイル名が指定されていません");
    return EINVAL; // TODO: ここもう少しわかりやすく呼び出し側に伝えられないか？
  }

  // XXX: save_fileのエラーハンドリングを未実装
  int rc = save_file(ed->filename, &ed->buf); // 0: 成功, -1: 失敗&errnoをセット
  return rc;
}

// TODO: 書き込みに成功&元のファイル名がなかったらEditorのfileを更新する？
int cmd_write(Editor *ed, const char *path) {
  // XXX: クソ適当
  int rc = 0;
  rc = save_file(path, &ed->buf);
  if (rc == 0) {
    ed->filename = strdup(path);
  }

  return rc;
}
int cmd_delete(Editor *ed) { return ENOSYS; }
int cmd_goto(Editor *ed, int n1_based) { return ENOSYS; }
