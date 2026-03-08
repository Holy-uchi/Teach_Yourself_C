#include "list-processor.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t (*OP_FUNC)(int32_t, int32_t);
static int32_t op_add(int32_t a, int32_t b) { return a + b; }
static int32_t op_sub(int32_t a, int32_t b) { return a - b; }
static int32_t op_mul(int32_t a, int32_t b) { return a * b; }
static int32_t op_div(int32_t a, int32_t b) { return a / b; }

int lisp_eval(Cons *cons, int32_t *out) {
  // carに入るのはNUMBERかOPERATORかCONS
  // でも関数の最初はOPERATORであるべき。
  // cdrに入るのはCONSかNIL
  // 関数が呼ばれるのはcarにconsが入ってた時。(ループ中)
  if (!cons || !out || !cons->car || !cons->cdr) {
    return -1;
  }
  Value *car, *cdr;
  Cons next_cons;
  OP_FUNC operator;
  int rc;

  car = cons->car;
  cdr = cons->cdr;

  if (car->type != VAL_OPERATOR) {
    // consのcarは必ずoperator
    // エラー
    return -1;
  }

  OperatorType op_type = car->as.op;
  switch (op_type) {
  case OP_ADD:
    operator = &op_add;
    break;
  case OP_SUB:
    operator = &op_sub;
    break;
  case OP_MUL:
    operator = &op_mul;
    break;
  case OP_DIV:
    operator = &op_div;
    break;
  default:
    return -1;
  }

  if (cdr->type != VAL_CONS) {
    // エラー
    // オペレータの直後は少なくとも2つのオペランドが来るべき
    return -1;
  }

  next_cons = cdr->as.cons;

  // 次のcarはNUMBERかCONSであるべき
  car = next_cons.car;
  cdr = next_cons.cdr;
  if (!car || !cdr) {
    return -1;
  }
  if (car->type != VAL_NUMBER && car->type != VAL_CONS) {
    // エラー
    return -1;
  }

  int32_t current;
  if (car->type == VAL_CONS) {
    rc = lisp_eval(&car->as.cons, &current);
    if (rc != 0) {
      return rc;
    }
  } else {
    current = car->as.number;
  }

  bool has_second_operand = false;
  while (cdr->type != VAL_NIL) {
    has_second_operand = true;
    if (cdr->type != VAL_CONS) {
      // エラー
      return -1;
    }
    next_cons = cdr->as.cons;
    car = next_cons.car;
    cdr = next_cons.cdr;
    if (!car || !cdr) {
      return -1;
    }

    if (car->type != VAL_NUMBER && car->type != VAL_CONS) {
      // エラー
      return -1;
    }
    int32_t as_num;
    if (car->type == VAL_CONS) {
      rc = lisp_eval(&car->as.cons, &as_num);
      if (rc != 0) {
        return rc;
      }
    } else {
      as_num = car->as.number;
    }
    if (op_type == OP_DIV && as_num == 0) {
      // 0除算
      return -1;
    }
    current = operator(current, as_num);
  }

  if (!has_second_operand) {
    return -1;
  }
  *out = current;
  return 0;
}

// XXX: これ多分使わない。
// int cons_new(Cons **out) {
//   if (!out || *out != NULL) {
//     // *outはNULLを要求
//     return EINVAL;
//   }
//   // car == NULL や cdr == NULL は許容
//   Cons *tmp = malloc(sizeof(Cons));
//   if (!tmp) {
//     return ENOMEM;
//   }
//
//   tmp->car = tmp->cdr = NULL;
//   *out = tmp;
//   return 0;
// }

void cons_init(Cons *cons) {
  cons->car = NULL;
  cons->cdr = NULL;
}

void cons_destroy(Cons cons, bool recursive) {
  value_delete(&cons.car, recursive);
  value_delete(&cons.cdr, recursive);
}

int value_new_with_num(int32_t num, Value **out) {
  if (!out || *out != NULL) {
    // *outはNULLを要求
    return EINVAL;
  }

  Value *tmp = malloc(sizeof(Value));
  if (!tmp) {
    return ENOMEM;
  }

  tmp->type = VAL_NUMBER;
  tmp->as.number = num;

  *out = tmp;
  return 0;
}

int value_new_with_op(OperatorType op, Value **out) {
  if (!out || *out != NULL) {
    // *outはNULLを要求
    return EINVAL;
  }

  Value *tmp = malloc(sizeof(Value));
  if (!tmp) {
    return ENOMEM;
  }
  tmp->type = VAL_OPERATOR;
  tmp->as.op = op;

  *out = tmp;
  return 0;
}

int value_new_with_nil(Value **out) {
  if (!out || *out != NULL) {
    // *outはNULLを要求
    return EINVAL;
  }

  Value *tmp = malloc(sizeof(Value));
  if (!tmp) {
    return ENOMEM;
  }
  tmp->type = VAL_NIL;

  *out = tmp;
  return 0;
}

int value_new_with_cons(const Cons cons, Value **out) {
  if (!out || *out != NULL) {
    // *outはNULLを要求
    return EINVAL;
  }

  Value *tmp = malloc(sizeof(Value));
  if (!tmp) {
    return ENOMEM;
  }
  tmp->type = VAL_CONS;
  tmp->as.cons = cons;

  *out = tmp;
  return 0;
}

void value_delete(Value **value, bool recursive) {
  if (!value) {
    return;
  }
  if (*value == NULL) {
    return;
  }
  if (!recursive) {
    free(*value);
    *value = NULL;
    return;
  }

  // recursive == true
  switch ((*value)->type) {
  case VAL_NUMBER:
  case VAL_OPERATOR:
  case VAL_NIL:
    free(*value);
    *value = NULL;
    return;
  case VAL_CONS:
    cons_destroy((*value)->as.cons, recursive);
    free(*value);
    *value = NULL;
    return;
  }
}
