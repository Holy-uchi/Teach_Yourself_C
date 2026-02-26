// CMAKE_SKIP_EXE
#include <stddef.h>
#include <stdlib.h>
#include <sys/errno.h>

int format_msg(char *buf, size_t cap, size_t *out_needed);

int main(void) {

  size_t needed = 0;
  int rc = format_msg(NULL, 0, &needed);
  if (rc != 0) {
    return rc;
  }
  char *buf = malloc(needed);
  if (!buf) {
    return ENOMEM;
  }
  rc = format_msg(buf, needed, &needed);
  if (rc != 0) {
    free(buf);
    return rc;
  }

  // bufを使う

  free(buf);
  return 0;
}
