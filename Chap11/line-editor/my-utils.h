#pragma once
#include "line-vector.h"
#include <stddef.h>

typedef struct {
  char *p;
  size_t len;
  size_t cap;
} StrBuf;

void sb_init(StrBuf *sb);
void sb_destroy(StrBuf *sb);

int sb_reserve(StrBuf *sb, size_t min_size);
int sb_append(StrBuf *sb, char *str);
int sb_push_char(StrBuf *sb, char ch);

int parse_line(const char *line, LineVec *tk);
