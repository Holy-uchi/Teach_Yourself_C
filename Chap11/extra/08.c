#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  const char *key;
  const char *val; // cfg内部の文字列（borrowed）
} Entry;

typedef struct {
  Entry *entries;
  size_t n;
} Config;

// 所与：cfg内部の文字列ポインタを返す（borrowed / free禁止）
const char *config_get(const Config *cfg, const char *key) {
  if (!cfg || !key)
    return NULL;
  for (size_t i = 0; i < cfg->n; i++) {
    if (strcmp(cfg->entries[i].key, key) == 0) {
      return cfg->entries[i].val;
    }
  }
  return NULL;
}

// 所与：srcをmallocしてコピーして*outに返す（owned / 呼び出し側がfree）
int make_copy(const char *src, char **out) {
  if (!src || !out || *out != NULL)
    return EINVAL;

  size_t n = strlen(src) + 1;
  char *p = malloc(n);
  if (!p)
    return ENOMEM;
  memcpy(p, src, n);

  *out = p;
  return 0;
}

/*
 * TODO（あなたの担当）
 * config_get の戻り（borrowed）を make_copy で owned に変換して返す。
 *
 * 契約案：
 * - 成功：return 0, *out_owned に malloc した文字列（呼び出し側が free）
 * - key が無い：return ENOENT, *out_owned は NULL のまま
 * - 失敗：errno系を返し、*out_owned は NULL のまま
 *
 * 注意：
 * - borrowed を free しない
 * - 失敗時に *out_owned を汚さない（NULL保証）
 */
int config_get_owned(const Config *cfg, const char *key, char **out_owned) {
  // TODO: ここを実装
  if (!cfg || !key || !out_owned || *out_owned != NULL) {
    return EINVAL;
  }
  const char *val = config_get(cfg, key);
  if (!val) {
    return ENOENT;
  }

  char *tmp = NULL;
  int rc = make_copy(val, &tmp);
  if (rc != 0) {
    return rc;
  }

  *out_owned = tmp;
  return 0;
}

int main(void) {
  Entry entries[] = {
      {"host", "example.com"},
      {"port", "443"},
  };
  Config cfg = {.entries = entries, .n = 2};

  int rc = 0;

  // 1) あるキー：owned を受け取って free まで
  char *host = NULL;
  rc = config_get_owned(&cfg, "host", &host);
  if (rc != 0) {
    fprintf(stderr, "config_get_owned(host) failed: %s\n", strerror(rc));
    return 1;
  }
  printf("host=%s\n", host);
  free(host);

  // 2) ないキー：ENOENT で落ちる（owned は NULL のまま）
  char *missing = NULL;
  rc = config_get_owned(&cfg, "missing", &missing);
  if (rc != ENOENT || missing != NULL) {
    fprintf(stderr, "expected ENOENT and NULL, got rc=%d missing=%p\n", rc, (void *)missing);
    free(missing); // 念のため
    return 1;
  }

  puts("OK");
  return 0;
}
