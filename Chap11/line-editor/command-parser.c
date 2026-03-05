#include "command-parser.h"
#include "line-vector.h"
#include "my-utils.h"
#include <_string.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

void command_init(COMMAND *cmd) {
  cmd->valid = false;
  cmd->type = '\0';
  cmd->line = 0;
  cmd->path = NULL;
}

void command_destroy(COMMAND *cmd) {
  free(cmd->path);
  cmd->path = NULL;
  cmd->valid = false;
  cmd->type = '\0'; // TODO: enumにINVALIDみたいなcommand_typeを追加すればいいのでは？
  cmd->line = 0;
}

int parse_command(const char *line, COMMAND *cmd) {
  if (!line || !cmd) {
    return EINVAL;
  }
  // XXX: クソ適当
  // TODO: リークや二重解放、ダングリングポインタがないかチェック

  // ====リソース確保====
  int rc = 0;
  COMMAND cmd_local = {0};
  command_init(&cmd_local);
  char *tmp_path = NULL;
  LineVec tok_vec = {0};
  lv_init(&tok_vec);
  // ====================

  rc = parse_line(line, &tok_vec);
  if (rc != 0) {
    goto cleanup;
  }

  if (tok_vec.len == 0) {
    // 入力が""の時。
    cmd->valid = false;
  } else if (strcmp(tok_vec.p[0], "a") == 0) {
    cmd_local.valid = true;
    cmd_local.type = APPEND;
    *cmd = cmd_local;
  } else if (strcmp(tok_vec.p[0], "r") == 0) {
    cmd_local.valid = true;
    cmd_local.type = REPLACE;
    *cmd = cmd_local;
  } else if (strcmp(tok_vec.p[0], "l") == 0) {
    cmd_local.valid = true;
    cmd_local.type = LIST;
    *cmd = cmd_local;
  } else if (strcmp(tok_vec.p[0], "q") == 0) {
    cmd_local.valid = true;
    cmd_local.type = QUIT;
    *cmd = cmd_local;
  } else if (strcmp(tok_vec.p[0], "w") == 0) {
    if (tok_vec.len < 2) {
      // 空白の後にパスが入力されてない場合は不正
      // FIXME: ここのエラーメッセージどうする？
      rc = EINVAL;
      goto cleanup;
    }
    cmd_local.valid = true;
    cmd_local.type = WRITE;
    cmd_local.path = strdup(tok_vec.p[1]);
    *cmd = cmd_local;
    command_init(&cmd_local); // cleanupで破壊しないように
  } else if (strcmp(tok_vec.p[0], "s") == 0) {
    cmd_local.valid = true;
    cmd_local.type = SAVE;
    *cmd = cmd_local;
  } else if (strcmp(tok_vec.p[0], "d") == 0) {
    cmd_local.valid = true;
    cmd_local.type = DELETE;
    *cmd = cmd_local;
  } else if (strcmp(tok_vec.p[0], "g") == 0) {
    if (tok_vec.len < 2) {
      // 空白の後にパスが入力されてない場合は不正
      // FIXME: ここのエラーメッセージどうする？
      rc = EINVAL;
      goto cleanup;
    }
    long line_no = 0;
    rc = parse_token_to_decimal(tok_vec.p[1], &line_no);
    if (rc != 0) {
      // FIXME: ここのエラーメッセージどうする？
      goto cleanup;
    }
    cmd_local.valid = true;
    cmd_local.type = GOTO;
    cmd_local.line = line_no;
    *cmd = cmd_local;
  }

cleanup:
  command_destroy(&cmd_local);
  lv_destroy(&tok_vec);
  free(tmp_path);
  return rc;
}
