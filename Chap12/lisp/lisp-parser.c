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
    pos_local++;
  } else if (tv->p[pos_local].type == TOK_OPERATOR) {
    rc = value_new_with_op(tv->p[pos_local].as.op, &val_tmp);
    if (rc != 0) {
      goto cleanup;
    }
    pos_local++;
  } else if (tv->p[pos_local].type == TOK_LPAREN) {
    rc = parse_list(tv, &pos_local, &val_tmp);
    if (rc != 0) {
      goto cleanup;
    }
  } else {
    rc = EINVAL;
    goto cleanup;
  }

  *out = val_tmp;
  val_tmp = NULL;

cleanup:
  *pos = pos_local;
  value_delete(&val_tmp, true);
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
  size_t pos_local = *pos;
  Value *car_val = NULL;
  Value *root = NULL;
  Value *tail = NULL;
  Value *new_node = NULL;

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
    rc = value_new_with_cons(next_cons, &new_node);
    if (rc != 0) {
      // val_tmpはNULL
      // next_consは.carがヒープ
      goto cleanup;
    }
    car_val = NULL;
    if (root == NULL) {
      // 最初のconsへの参照を残す
      root = new_node;
    } else {
      tail->as.cons.cdr = new_node;
    }
    tail = new_node;
    new_node = NULL;
  }

  if (pos_local == tv->len) {
    // 入力末尾まで来たのに対応するTOK_RPARENが見つからない
    rc = EINVAL;
    goto cleanup;
  }
  if (tv->p[pos_local].type != TOK_RPAREN) {
    // 少し冗長だが、末尾の')'チェック
    rc = EINVAL;
    goto cleanup;
  }

  // もしroot_cons_valがないなら、listが空だったと言うこと
  if (root == NULL) {
    // 空リストは不正とする
    rc = EINVAL;
    goto cleanup;
  }

  // pos_local < tv->len && TOK_RPAREN
  // TOK_RPARENの分１つ消費
  pos_local++;
  rc = value_new_with_nil(&new_node);
  if (rc != 0) {
    goto cleanup;
  }
  tail->as.cons.cdr = new_node;

  *out = root;
  car_val = NULL;
  root = NULL;
  new_node = NULL;

cleanup:
  *pos = pos_local;
  value_delete(&car_val, true);
  value_delete(&new_node, true);
  value_delete(&root, true);
  return rc;
}
