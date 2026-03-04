#pragma once
#include "line-editor.h"

// cmd_append(ed, in)
// cmd_replace(ed, in)
// cmd_list(ed, out)
// cmd_write(ed, path)
// cmd_save(ed)
// cmd_delete(ed)
// cmd_goto(ed, n1_based)

int cmd_append(Editor *ed, FILE *in);
int cmd_replace(Editor *ed, FILE *in);
int cmd_list(Editor *ed, FILE *out);
int cmd_write(Editor *ed, const char *path);
int cmd_save(Editor *ed);
int cmd_delete(Editor *ed);
int cmd_goto(Editor *ed, int n1_based);
