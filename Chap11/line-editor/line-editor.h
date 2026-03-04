#pragma once
#include "line-vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
  LineVec buf;
  size_t cur;
  char *filename;
} Editor;

void editor_init(Editor *ed);
void editor_destroy(Editor *ed);

int editor_open(Editor *ed, const char *filename_or_null); // 起動時のロード処理
int editor_run(Editor *ed, FILE *out);                     // REPL本体
