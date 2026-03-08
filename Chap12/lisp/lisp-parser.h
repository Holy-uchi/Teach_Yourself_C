#pragma once
#include "lisp-tokenizer.h"
#include "list-processor.h"
#include <stdint.h>

int parse_value(const TokenVec *tv, size_t *pos, Value **out);
int parse_list(const TokenVec *tv, size_t *pos, Value **out);
