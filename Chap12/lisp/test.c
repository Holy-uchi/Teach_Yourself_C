
#include "list-processor.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* lisp_eval は list-processor.c 側で実装済み想定 */
int lisp_eval(Cons *cons, int32_t *out);

/* ---------- テスト用ヘルパ ---------- */

static Value *alloc_value(void) {
  Value *v = (Value *)malloc(sizeof(Value));
  if (!v) {
    fprintf(stderr, "malloc failed\n");
    exit(1);
  }
  return v;
}

static Value *make_number(int32_t n) {
  Value *v = alloc_value();
  v->type = VAL_NUMBER;
  v->as.number = n;
  return v;
}

static Value *make_operator(OperatorType op) {
  Value *v = alloc_value();
  v->type = VAL_OPERATOR;
  v->as.op = op;
  return v;
}

static Value *make_nil(void) {
  Value *v = alloc_value();
  v->type = VAL_NIL;
  return v;
}

static Value *make_cons(Value *car, Value *cdr) {
  Value *v = alloc_value();
  v->type = VAL_CONS;
  v->as.cons.car = car;
  v->as.cons.cdr = cdr;
  return v;
}

/* Value 再帰解放 */
static void free_value(Value *v) {
  if (!v) {
    return;
  }
  if (v->type == VAL_CONS) {
    free_value(v->as.cons.car);
    free_value(v->as.cons.cdr);
  }
  free(v);
}

/*
 * 2項式: (op a b)
 */
static Value *make_expr2(OperatorType op, Value *a, Value *b) {
  Value *nil = make_nil();
  Value *node2 = make_cons(b, nil);
  Value *node1 = make_cons(a, node2);
  return make_cons(make_operator(op), node1);
}

/*
 * 3項式: (op a b c)
 */
static Value *make_expr3(OperatorType op, Value *a, Value *b, Value *c) {
  Value *nil = make_nil();
  Value *node3 = make_cons(c, nil);
  Value *node2 = make_cons(b, node3);
  Value *node1 = make_cons(a, node2);
  return make_cons(make_operator(op), node1);
}

/*
 * 4項式: (op a b c d)
 */
static Value *make_expr4(OperatorType op, Value *a, Value *b, Value *c, Value *d) {
  Value *nil = make_nil();
  Value *node4 = make_cons(d, nil);
  Value *node3 = make_cons(c, node4);
  Value *node2 = make_cons(b, node3);
  Value *node1 = make_cons(a, node2);
  return make_cons(make_operator(op), node1);
}

/* ---------- テストマクロ ---------- */

static int total_tests = 0;
static int passed_tests = 0;

#define EXPECT_OK(expr, expected)                                                                  \
  do {                                                                                             \
    total_tests++;                                                                                 \
    int32_t out = 0;                                                                               \
    int rc = lisp_eval(&(expr)->as.cons, &out);                                                    \
    if (rc == 0 && out == (expected)) {                                                            \
      passed_tests++;                                                                              \
      printf("[PASS] %s => rc=%d out=%d\n", #expr, rc, out);                                       \
    } else {                                                                                       \
      printf("[FAIL] %s => rc=%d out=%d expected=%d\n", #expr, rc, out, (expected));               \
    }                                                                                              \
    free_value(expr);                                                                              \
  } while (0)

#define EXPECT_ERR(expr)                                                                           \
  do {                                                                                             \
    total_tests++;                                                                                 \
    int32_t out = 0;                                                                               \
    int rc = lisp_eval(&(expr)->as.cons, &out);                                                    \
    if (rc != 0) {                                                                                 \
      passed_tests++;                                                                              \
      printf("[PASS] %s => error rc=%d\n", #expr, rc);                                             \
    } else {                                                                                       \
      printf("[FAIL] %s => expected error but rc=0 out=%d\n", #expr, out);                         \
    }                                                                                              \
    free_value(expr);                                                                              \
  } while (0)

/* ---------- テストケース ---------- */

static void test_add_2args(void) {
  Value *expr = make_expr2(OP_ADD, make_number(1), make_number(2));
  EXPECT_OK(expr, 3);
}

static void test_add_4args(void) {
  Value *expr = make_expr4(OP_ADD, make_number(1), make_number(2), make_number(3), make_number(4));
  EXPECT_OK(expr, 10);
}

static void test_sub_3args(void) {
  /* (- 10 3 2) => (10 - 3) - 2 = 5 */
  Value *expr = make_expr3(OP_SUB, make_number(10), make_number(3), make_number(2));
  EXPECT_OK(expr, 5);
}

static void test_mul_3args(void) {
  Value *expr = make_expr3(OP_MUL, make_number(2), make_number(3), make_number(4));
  EXPECT_OK(expr, 24);
}

static void test_div_3args(void) {
  /* (/ 100 5 2) => (100 / 5) / 2 = 10 */
  Value *expr = make_expr3(OP_DIV, make_number(100), make_number(5), make_number(2));
  EXPECT_OK(expr, 10);
}

static void test_nested_simple(void) {
  /* (* (+ 1 2) 4) => 12 */
  Value *inner = make_expr2(OP_ADD, make_number(1), make_number(2));
  Value *expr = make_expr2(OP_MUL, inner, make_number(4));
  EXPECT_OK(expr, 12);
}

static void test_nested_both_sides(void) {
  /* (+ (* 2 3) (- 10 4)) => 6 + 6 = 12 */
  Value *left = make_expr2(OP_MUL, make_number(2), make_number(3));
  Value *right = make_expr2(OP_SUB, make_number(10), make_number(4));
  Value *expr = make_expr2(OP_ADD, left, right);
  EXPECT_OK(expr, 12);
}

static void test_nested_variadic(void) {
  /* (+ 1 (* 2 3 4) 5) => 1 + 24 + 5 = 30 */
  Value *inner = make_expr3(OP_MUL, make_number(2), make_number(3), make_number(4));
  Value *expr = make_expr3(OP_ADD, make_number(1), inner, make_number(5));
  EXPECT_OK(expr, 30);
}

static void test_div_by_zero_direct(void) {
  Value *expr = make_expr2(OP_DIV, make_number(10), make_number(0));
  EXPECT_ERR(expr);
}

static void test_div_by_zero_nested(void) {
  /* (+ 1 (/ 10 0)) => エラー */
  Value *inner = make_expr2(OP_DIV, make_number(10), make_number(0));
  Value *expr = make_expr2(OP_ADD, make_number(1), inner);
  EXPECT_ERR(expr);
}

static void test_too_few_operands(void) {
  /* (+ 1) */
  Value *nil = make_nil();
  Value *arg1 = make_cons(make_number(1), nil);
  Value *expr = make_cons(make_operator(OP_ADD), arg1);
  EXPECT_ERR(expr);
}

static void test_head_is_not_operator(void) {
  /* (123 1 2) */
  Value *nil = make_nil();
  Value *arg2 = make_cons(make_number(2), nil);
  Value *arg1 = make_cons(make_number(1), arg2);
  Value *expr = make_cons(make_number(123), arg1);
  EXPECT_ERR(expr);
}

static void test_first_operand_invalid(void) {
  /* (+ OP_ADD 2) 的な壊れた入力 */
  Value *expr = make_expr2(OP_ADD, make_operator(OP_SUB), make_number(2));
  EXPECT_ERR(expr);
}

static void test_later_operand_invalid(void) {
  /* (+ 1 OP_SUB) 的な壊れた入力 */
  Value *expr = make_expr2(OP_ADD, make_number(1), make_operator(OP_SUB));
  EXPECT_ERR(expr);
}

static void test_cdr_is_nil_immediately(void) {
  /* (+) */
  Value *expr = make_cons(make_operator(OP_ADD), make_nil());
  EXPECT_ERR(expr);
}

static void test_negative_numbers(void) {
  /* (+ -3 7) => 4 */
  Value *expr = make_expr2(OP_ADD, make_number(-3), make_number(7));
  EXPECT_OK(expr, 4);
}

/* ---------- test ---------- */

int test(void) {
  test_add_2args();
  test_add_4args();
  test_sub_3args();
  test_mul_3args();
  test_div_3args();
  test_nested_simple();
  test_nested_both_sides();
  test_nested_variadic();
  test_div_by_zero_direct();
  test_div_by_zero_nested();
  test_too_few_operands();
  test_head_is_not_operator();
  test_first_operand_invalid();
  test_later_operand_invalid();
  test_cdr_is_nil_immediately();
  test_negative_numbers();

  printf("\nResult: %d/%d tests passed\n", passed_tests, total_tests);
  return (passed_tests == total_tests) ? 0 : 1;
}
