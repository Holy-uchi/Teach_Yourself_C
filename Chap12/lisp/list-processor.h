#pragma once
#include <stdbool.h>
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

void cons_init(Cons *cons);
void cons_destroy(Cons cons, bool recursive);

int value_new_with_num(int32_t num, Value **out);
int value_new_with_op(OperatorType op, Value **out);
int value_new_with_nil(Value **out);
int value_new_with_cons(const Cons cons, Value **out);
void value_delete(Value **value, bool recursive);
