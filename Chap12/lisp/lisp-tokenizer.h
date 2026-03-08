#pragma once
#include "list-processor.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
  char **p;
  size_t len;
  size_t cap;
} LineVec;

typedef enum {
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_OPERATOR,
  TOK_NUMBER,
} TokenType;

typedef struct {
  TokenType type;
  union {
    int32_t number;
    OperatorType op;
  } as;
} LispToken;

typedef struct {
  LispToken *p;
  size_t len;
  size_t cap;
} TokenVec;

void lv_init(LineVec *lv);
void lv_destroy(LineVec *lv);
int lv_reserve(LineVec *lv, size_t min_size);
int lv_push(LineVec *lv, char *str);

void tv_init(TokenVec *tv);
void tv_destroy(TokenVec *tv);
int tv_reserve(TokenVec *tv, size_t min_size);
int tv_push(TokenVec *tv, LispToken token);

int segment_string(const char *buf, LineVec *lv_out);
int tokenize_segments(const LineVec *segments, TokenVec *tv_out);

int safe_strtoi32(const char *str, int32_t *out);
