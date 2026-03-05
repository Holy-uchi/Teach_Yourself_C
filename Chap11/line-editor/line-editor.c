// typedef struct {
//   LineVec buf;
//   size_t cur;
//   char *filename;
// } Editor;
#include "line-editor.h"
#include "command-handler.h"
#include "command-parser.h"
#include "line-vector.h"
#include "load-file.h"
#include "read-line.h"
#include "save-file.h"
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void editor_init(Editor *ed) {
  lv_init(&ed->buf);
  ed->cur = 0;
  ed->filename = NULL;
}

void editor_destroy(Editor *ed) {
  lv_destroy(&ed->buf);
  ed->cur = 0;
  free(ed->filename);
  ed->filename = NULL;
}

int editor_open(Editor *ed, const char *filename_or_null) {
  // XXX: エラーコードの設計がない
  if (!ed) {
    return EINVAL;
  }
  // *edは初期化されている前提
  int rc = 0;
  LineVec lv_tmp = {0};
  lv_init(&lv_tmp);

  if (filename_or_null != NULL) {
    rc = load_file(filename_or_null, &lv_tmp);
    if (rc < 0) {
      perror("load-file");
      goto cleanup;
    }
    ed->buf = lv_tmp;
    lv_init(&lv_tmp); // cleanupでfreeしないように。
    ed->filename = strdup(filename_or_null);
  } else {
    // filename_or_null == NULL
    // 初期化直後のedを渡されたならfilenameはnullのはずだが、明示的にnullにしておく
    ed->filename = NULL;
  }

cleanup:
  lv_destroy(&lv_tmp);
  return rc;
}

int editor_run(Editor *ed, FILE *out) {
  // XXX: errorコードの設計をしていない。
  if (!ed || !out) {
    return EINVAL;
  }
  int rc = 0;
  char *line = NULL;
  COMMAND cmd = {0};
  command_init(&cmd);
  bool finish_loop = false;
  // コマンドを待ち続ける
  for (;;) {
    // XXX: クソ適当
    // TODO: リークしてないかレビュー
    // TODO: 二重解放もしてないかレビュー。
    if ((rc = read_line(stdin, &line)) != 1) {
      // 1: 成功, 0: EOF, -1: エラー
      if (rc < 0) {
        perror("parse-command");
      }
      goto cleanup;
    }
    // NOTE: lineは "" もあり得る。
    parse_command(line, &cmd);
    if (!cmd.valid) {
      fputs("不正なコマンドです\n", out);
    } else {
      // 正しいコマンド
      switch (cmd.type) {
      case APPEND:
        rc = cmd_append(ed, stdin);
        if (rc < 0) {
          perror("append failed");
          // XXX: 中断べき？続行すべき？
        }
        break;
      case REPLACE:
        rc = cmd_replace(ed, stdin);
        if (rc < 0) {
          perror("replace failed");
          // XXX: 中断べき？続行すべき？
        }
        break;
      case LIST:
        rc = cmd_list(ed, stdout);
        if (rc < 0) {
          perror("list failed");
          // XXX: 中断べき？続行すべき？
        }
        break;
      case WRITE:
        rc = cmd_write(ed, cmd.path);
        if (rc < 0) {
          perror("write failed");
          // XXX: 中断べき？続行すべき？
        }
        break;
      case SAVE:
        rc = cmd_write(ed, ed->filename); // XXX: 引数これであってる？
        if (rc < 0) {
          if (errno) {
            perror("save failed");
          }
          // NOTE: 中断しない
        }
        break;
      case DELETE:
        rc = cmd_delete(ed);
        if (rc < 0) {
          perror("delete failed");
          // XXX: 中断すべき？続行すべき？
        }
        break;
      case GOTO:
        rc = cmd_goto(ed, cmd.line); // XXX: 引数これであってる？cmd->lineは1basedなの？
        if (rc < 0) {
          perror("goto failed");
        }
        break;
      case QUIT:
        // INFO: done

        finish_loop = true;
        break;
      }
    }

    if (finish_loop) {
      // ループ終了
      break;
    } else {
      // ループ継続
      free(line);
      line = NULL;
      command_destroy(&cmd);
      command_init(&cmd);
    }
  }
cleanup:
  free(line);
  command_destroy(&cmd);
  return rc;
}
