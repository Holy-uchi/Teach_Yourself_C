#include "command-parser.h"
#include "line-editor.h"
#include "line-vector.h"
#include "read-line.h"
#include "save-file.h"
#include <_string.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

static int append_or_replace(Editor *ed, FILE *in, COMMAND_TYPE cmd_type) {
  if (!ed || !in) {
    return EINVAL;
  }

  if (cmd_type != APPEND && cmd_type != REPLACE) {
    return EINVAL;
  }

  int rc = 0;
  char *line = NULL;
  LineVec lv_local = {0};
  lv_init(&lv_local);
  LineVec lv_tmp_new = {0};
  lv_init(&lv_tmp_new);
  const char *cmd_name = cmd_type == APPEND ? "append" : "replace";

  // TODO: input_loopみたいなヘルパー欲しい
  for (;;) {

    // "." が来るまでの間LineVecに格納し続ける
    rc = read_line(in, &line); // 1: 成功, 0: EOF, -1: 失敗+errno
    if (rc < 0) {
      fprintf(stderr, "%s failed\n", cmd_name);
      goto cleanup;
    } else if (rc == 0) {
      // EOFにはならないはずだが
      fprintf(stderr, "%s failed with unexpected error", cmd_name);
      goto cleanup;
    }

    // 行頭に.が入力されたら終了
    size_t len = strlen(line);
    // ""を考慮
    if (len > 0 && line[0] == '.') {
      free(line);
      line = NULL;
      break;
    }

    // lv_localにpush
    rc = lv_push(&lv_local, line); // 所有権を渡す
    if (rc != 0) {
      fprintf(stderr, "lv_local push failed while %s: %s\n", cmd_name, strerror(rc));
      goto cleanup;
    }
    // 次のloopに備えて一応外でもNULL化
    line = NULL;
  }

  // .で正常にループを抜けた場合。(line解放済み)

  // 0 ~ ed->cur までed->bufからとる <<< APPENDの時
  // 0 ~ ed->cur - 1までed->bufからとる <<< REPLACEの時
  // lv_localから全部とる
  // ed->cur からed->buf.lenまで残りをとる
  // ed->bufをdestroyしてlv_tmp_newに付け替える
  // ed->curをlv_local.lenだけインクリメント

  // NOTE: ed->buf.len == 0を考慮
  // TODO: lv_mergeみたいなヘルパー欲しい。

  for (size_t i = 0; (i <= ed->cur) && (i < ed->buf.len); i++) {
    if (cmd_type == REPLACE && i >= ed->cur) {
      // NOTE: REPLACEの時はcur行を置き換えるので, ed->cur-1で終了
      break;
    }
    line = strdup(ed->buf.p[i]);
    rc = lv_push(&lv_tmp_new, line);
    if (rc != 0) {
      fprintf(stderr, "lv_tmp_new push failed: %s\n", strerror(rc));
      goto cleanup;
    }
    line = NULL;
  }

  for (size_t i = 0; i < lv_local.len; i++) {
    line = strdup(lv_local.p[i]);
    rc = lv_push(&lv_tmp_new, line);
    if (rc != 0) {
      fprintf(stderr, "lv_tmp_new push failed: %s\n", strerror(rc));
      goto cleanup;
    }
    line = NULL;
  }
  for (size_t i = ed->cur + 1; i < ed->buf.len; i++) {
    line = strdup(ed->buf.p[i]);
    rc = lv_push(&lv_tmp_new, line);
    if (rc != 0) {
      fprintf(stderr, "lv_tmp_new push failed: %s\n", strerror(rc));
      goto cleanup;
    }
    line = NULL;
  }
  // lv_tmp_newは完成したLineVecになっている
  lv_destroy(&ed->buf);
  ed->buf = lv_tmp_new;
  lv_init(&lv_tmp_new); // cleanupで破壊しないように初期化
  // NOTE: append後にcursol位置が変わるかどうかは仕様にない
  // NOTE: REPLACEの時とAPPENDの時でcursor位置が変わる
  size_t diff = cmd_type == APPEND ? lv_local.len : lv_local.len - 1;
  ed->cur += diff;

cleanup:
  free(line);
  lv_destroy(&lv_local);
  lv_destroy(&lv_tmp_new);
  return rc;
}

int cmd_append(Editor *ed, FILE *in) { return append_or_replace(ed, in, APPEND); }

int cmd_replace(Editor *ed, FILE *in) { return append_or_replace(ed, in, REPLACE); }

int cmd_list(Editor *ed, FILE *out) {
  if (!ed || !out) {
    return EINVAL;
  }
  int rc = 0;

  // 不正パターン
  // 長さが0 => 不正というよりそれを伝えたいが、、、
  if (ed->buf.len == 0) {
    fputs("== This file has no lines so far ==", out);
    return 0;
  }

  size_t start, last;
  start = ed->cur >= 2 ? ed->cur - 2 : 0; // 現在のカーソルより前に2行あるなら cur - 2
  last = ed->buf.len >= (ed->cur + 1) + 2
             ? ed->cur + 2
             : ed->buf.len - 1; // 現在のカーソルより後ろに2行以上あるなら cur + 2

  for (size_t i = start; i <= last; i++) {
    // XXX: 4桁以上の行がある時行番号がズレるし、3桁以下の時ちょっと変
    // FIXME: というか切り詰められる
    if (i == ed->cur) {
      fprintf(out, "%3zu* %s\n", i + 1 /* 1-based*/, ed->buf.p[i]);
    } else {
      fprintf(out, "%3zu  %s\n", i + 1 /* 1-based*/, ed->buf.p[i]);
    }
  }

  return rc;
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
  // TODO: 「保存しました」って出したい
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

int cmd_delete(Editor *ed) {
  if (ed->buf.len == 0) {
    // TODO: エラーメッセージはどっちが出す？
    return EINVAL;
  }
  lv_delete(&ed->buf, ed->cur);
  return 0;
}
int cmd_goto(Editor *ed, int n1_based) { return ENOSYS; }
