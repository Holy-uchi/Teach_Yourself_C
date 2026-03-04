#include "command-parser.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

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
  cmd->type = '\0'; // FIXME: enumにINVALIDみたいなcommand_typeを追加すればいいのでは？
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
  // ====================

  if (line[0] == 'a') {
    cmd_local.valid = true;
    cmd_local.type = APPEND;
    *cmd = cmd_local;
  } else if (line[0] == 'q') {
    cmd_local.valid = true;
    cmd_local.type = QUIT;
    *cmd = cmd_local;
  } else if (line[0] == 'w') {
    // sscanfかな？
    // cmd->pathに空白の跡を埋める
    tmp_path = malloc(64);
    if (!tmp_path) {
      rc = ENOMEM;
      goto cleanup;
    }

    int n = sscanf(line, "w %63s ", tmp_path);
    // ファイル名が63文字以上なら？ => ENOBUF
    // tmp_pathが読み取れてないなら？ => EINVALかな？
    // XXX: この辺からマジでこんがらがってくる
    if (n < 2) {
      rc = EINVAL;
      goto cleanup;
    }
  } else if (line[0] == 's') {
    cmd_local.valid = true;
    cmd_local.type = SAVE;
    *cmd = cmd_local;
  }

cleanup:
  command_destroy(&cmd_local);
  free(tmp_path);
  return rc;
}
