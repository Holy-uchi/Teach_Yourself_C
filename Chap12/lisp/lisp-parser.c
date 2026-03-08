#include "lisp-parser.h"
#include "lisp-tokenizer.h"
#include "list-processor.h"
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/errno.h>

int parse_value(const TokenVec *tv, size_t *pos, Value **out) {
  if (!tv || !pos || !out || (*out) != NULL) {
    return EINVAL;
  }
  if (*pos >= tv->len) {
    return ERANGE;
  }

  int rc = 0;
  size_t pos_local = *pos;
  Value *val_tmp = NULL;

  if (tv->p[pos_local].type == TOK_NUMBER) {
    rc = value_new_with_num(tv->p[pos_local].as.number, &val_tmp);
    if (rc != 0) {
      goto cleanup;
    }
  } else if (tv->p[pos_local].type == TOK_OPERATOR) {
    rc = value_new_with_op(tv->p[pos_local].as.op, &val_tmp);
    if (rc != 0) {
      goto cleanup;
    }
  } else if (tv->p[pos_local].type == TOK_LPAREN) {
    rc = parse_list(tv, &pos_local, &val_tmp);
    if (rc != 0) {
      goto cleanup;
    }
  } else {
    rc = EINVAL;
    puts("1");
    goto cleanup;
  }

  *out = val_tmp;
  val_tmp = NULL;

cleanup:
  *pos = pos_local;
  value_delete(&val_tmp, false);
  return rc;
}

int parse_list(const TokenVec *tv, size_t *pos, Value **out) {
  if (!tv || !pos || !out || (*out) != NULL) {
    return EINVAL;
  }

  if (*pos >= tv->len) {
    return ERANGE;
  }

  int rc = 0;
  size_t pos_local = *pos; // 直接インクリメントしながら進んだほうがいいのか？
  Value *car_val = NULL;
  Value *root_cons_val = NULL;
  Value *last_cons_val = NULL;
  Value *current_cons_val = NULL;

  // 一つ目がTOK_LPARENでないなら不正
  if (tv->p[pos_local].type != TOK_LPAREN) {
    rc = EINVAL;
    goto cleanup;
  }
  // '(' の分１文字消費
  pos_local++;

  while (pos_local < tv->len && tv->p[pos_local].type != TOK_RPAREN) {
    rc = parse_value(tv, &pos_local, &car_val);
    if (rc != 0) {
      // val_localはNULL
      goto cleanup;
    }
    Cons next_cons = {.car = car_val, .cdr = NULL};
    car_val = NULL;
    rc = value_new_with_cons(next_cons, &current_cons_val);
    if (rc != 0) {
      // val_tmpはNULL
      // next_consは.carがヒープ
      goto cleanup;
    }
    if (!root_cons_val) {
      // 最初のconsへの参照を残す
      root_cons_val = current_cons_val;
    }
    if (last_cons_val != NULL) {
      last_cons_val->as.cons.cdr = current_cons_val;
    }
    last_cons_val = current_cons_val;
    current_cons_val = NULL;

    pos_local++;
  }

  // もしtoken_vectorを最後まで走査したのにTOK_RPARENじゃないならカッコの数が不一致
  if (pos_local == tv->len && tv->p[pos_local].type != TOK_RPAREN) {
    rc = EINVAL;
    goto cleanup;
  }

  // もしroot_cons_valがないなら、listが空だったと言うこと
  if (!root_cons_val) {
    rc = EINVAL;
    goto cleanup;
  }

  // pos_local < tv->len && TOK_RPAREN
  rc = value_new_with_nil(&current_cons_val);
  if (rc != 0) {
    goto cleanup;
  }
  last_cons_val->as.cons.cdr = current_cons_val;

  *out = root_cons_val;
  car_val = NULL;
  root_cons_val = NULL;
  current_cons_val = NULL;
  last_cons_val = NULL;

cleanup:
  *pos = pos_local;
  value_delete(&car_val, false);
  value_delete(&root_cons_val, false);
  value_delete(&last_cons_val, false);
  value_delete(&current_cons_val, false);
  return rc;
}
