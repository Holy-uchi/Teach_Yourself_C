// CMAKE_SKIP_EXE
#include "line-editor.h"
#include <stddef.h>

int main(int argc, char *argv[]) {
  int rc = 0;
  Editor ed = {0};
  editor_init(&ed);

  if (argc >= 2) {
    rc = editor_open(&ed, argv[1]);
  } else {
    rc = editor_open(&ed, NULL);
  }
  if (rc) {
    goto cleanup;
  }

  rc = editor_run(&ed, stdout);

cleanup:
  editor_destroy(&ed);
  return rc;
}
