#pragma once
#include <stdint.h>

typedef enum {
  VAL_NUMBER,
  VAL_OPERATOR,
  VAL_CONS,
  VAL_NIL,
} ValueType;

typedef enum {
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
} OperatorType;

typedef struct Value Value;
typedef struct Cons Cons;

struct Cons {
  Value *car;
  Value *cdr;
};

struct Value {
  ValueType type;
  union {
    int32_t number;
    OperatorType op;
    Cons cons;
  } as;
};

int lisp_eval(Cons *cons, int32_t *out);
