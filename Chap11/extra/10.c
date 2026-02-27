#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 所与：内部側（受け取る側）の“保管場所”
 * - take_ownership はここにポインタを移して管理する
 * - 既に何か持っていたら、それは内部側が片付ける（リーク防止）
 */
static char *g_owned = NULL;

static void release_internal(void) {
  free(g_owned);
  g_owned = NULL;
}

/*
 * TODO（あなたの担当）
 *
 * void take_ownership(char **io_p);
 * 契約:
 * - *io_p が指すヒープ領域の所有権を内部（g_owned）に移す
 * - 呼び出し側の *io_p は NULL にする（以後 free しても安全）
 * - io_p == NULL は no-op でOK
 * - *io_p == NULL も no-op でOK
 * - 既に g_owned が何か持っていたら内部で free してから置き換える（リーク防止）
 */
void take_ownership(char **io_p) {
  // TODO: ここを実装
  if (!io_p || !*io_p)
    return;
  if (g_owned) {
    release_internal();
  }
  g_owned = *io_p;
  *io_p = NULL;
  return;
}

/************ ここから下は所与（触らない） ************/
static char *make_owned_str(const char *s) {
  size_t n = strlen(s) + 1;
  char *p = malloc(n);
  if (!p)
    return NULL;
  memcpy(p, s, n);
  return p;
}

int main(void) {
  int rc = 0;
  char *p = make_owned_str("Hello");
  if (!p)
    return 1;

  // before
  printf("before: p=%p, g_owned=%p\n", (void *)p, (void *)g_owned);

  // move（所有権移譲）
  take_ownership(&p);

  // after
  printf("after:  p=%p, g_owned=%p\n", (void *)p, (void *)g_owned);

  // 呼び出し側はもう free しない（しても free(NULL) で安全）
  free(p); // pはNULLのはず

  // 内部側が最後に解放
  release_internal();

  puts("OK");
  return rc;
}
