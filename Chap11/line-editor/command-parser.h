#pragma once
#include <stdbool.h>

typedef enum {
  APPEND,
  REPLACE,
  LIST,
  WRITE,
  SAVE,
  DELETE,
  GOTO,
  QUIT,
} COMMAND_TYPE;
// cmd_append(ed, in)
// cmd_replace(ed, in)
// cmd_list(ed, out)
// cmd_write(ed, path)
// cmd_save(ed)
// cmd_delete(ed)
// cmd_goto(ed, n1_based)

typedef struct {
  bool valid;
  COMMAND_TYPE type;
  int line;
  char *path;
} COMMAND;

void command_init(COMMAND *cmd);
void command_destroy(COMMAND *cmd);

int parse_command(const char *line, COMMAND *cmd);
