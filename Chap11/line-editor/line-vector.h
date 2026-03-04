#pragma once
#include <stddef.h>

typedef struct {
  char **p;
  size_t len;
  size_t cap;
} LineVec;

void lv_init(LineVec *lv);

void lv_destroy(LineVec *lv);

int lv_reserve(LineVec *lv, size_t min_size);

int lv_push(LineVec *lv, char *str);

int lv_insert(LineVec *lv, size_t i, char *str);

int lv_delete(LineVec *lv, size_t i);
