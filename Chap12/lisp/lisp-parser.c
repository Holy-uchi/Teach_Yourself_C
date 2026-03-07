#include "lisp-tokenizer.h"
#include "list-processor.h"

int parse_value(LineVec *lv, size_t *pos, Value **out);
int parse_list(LineVec *lv, size_t *pos, Value **out);
