#include "line-vector.h"
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lv_init(LineVec *lv) {
  lv->p = NULL;
  lv->len = 0;
  lv->cap = 0;
}

void lv_destroy(LineVec *lv) {

  for (size_t i = 0; i < lv->len; i++) {
    free(lv->p[i]);
  }
  free(lv->p);
  lv->p = NULL;
  lv->len = 0;
  lv->cap = 0;
}

int lv_reserve(LineVec *lv, size_t min_size) {
  if (!lv) {
    return EINVAL;
  }

  if (lv->cap >= min_size) {
    return 0;
  }

  size_t ncap = lv->cap ? lv->cap : 8;
  while (ncap < min_size) {
    if (ncap > SIZE_MAX / 2) {
      return EOVERFLOW;
    }
    ncap *= 2;
  }

  if (ncap > SIZE_MAX / sizeof(*lv->p)) {
    return EOVERFLOW;
  }

  char **tmp = realloc(lv->p, sizeof(char *) * ncap);
  if (!tmp) {
    return ENOMEM;
  }
  lv->p = tmp;
  tmp = NULL;
  lv->cap = ncap;

  return 0;
}

int lv_push(LineVec *lv, char *str) {
  if (!lv || !str) {
    return EINVAL;
  }

  if (lv->len > SIZE_MAX - 1) {
    return EOVERFLOW;
  }

  int rc = lv_reserve(lv, lv->len + 1);
  if (rc != 0) {
    return rc;
  }

  lv->p[lv->len] = str;
  str = NULL;
  lv->len++;
  return 0;
}

int lv_insert(LineVec *lv, size_t i, char *str) {
  if (!lv || !str) {
    return EINVAL;
  }

  if (i > lv->len) {
    return ERANGE;
  }

  if (lv->len > SIZE_MAX - 1) {
    return EOVERFLOW;
  }
  int rc = lv_reserve(lv, lv->len + 1);
  if (rc) {
    return rc;
  }

  // lv->lenとiが同じかも
  if (i < lv->len) {
    memmove(&lv->p[i + 1], &lv->p[i], sizeof(char *) * (lv->len - i));
  }
  lv->p[i] = str;
  str = NULL;
  lv->len++;
  return 0;
}

int lv_delete(LineVec *lv, size_t i) {
  if (!lv) {
    return EINVAL;
  }
  if (i >= lv->len) {
    return ERANGE;
  }

  free(lv->p[i]);
  if (i + 1 < lv->len) {
    memmove(&lv->p[i], &lv->p[i + 1], sizeof(char *) * (lv->len - i - 1));
  }
  lv->len--;

  return 0;
}
